#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include "Punto.hpp"
#include "Vector.hpp"
#include "Matriz.hpp"
#include "Objetos.hpp"
#include "RayTracer.hpp"

using namespace std;

//FUNCIONES Y METODOS
//Calcula la luz siguiendo la brdf de phong dados el vector incidente, el reflejado y la esfera con la que se ha intersectado
void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr){

    float cos_theta_r =  Vector::cosenoVector(omega_i,omega_r);
    if(cos_theta_r<0){
        cos_theta_r = 0;
    }

    float ks = lista_esferas[ultima_esfera]->getKs()*((2+ALPHA)/(2*PI))*pow(cos_theta_r,ALPHA); // Alpha = 5 constante
    fr[0] = lista_esferas[ultima_esfera]->getKd()[0]/PI + ks;
    fr[1] = lista_esferas[ultima_esfera]->getKd()[1]/PI + ks;
    fr[2] = lista_esferas[ultima_esfera]->getKd()[2]/PI + ks;
}
/**
 * Devolvera la iluminación indirecta por montecarlo
 * @param impacto
 * @param fr
 */
void iluminacion_indirecta(Punto interseccion, Vector normal, float* fr){

    Matriz T = calcular_locales(normal, interseccion);
    Matriz T_inversa = T.invertir();
    Esfera esfLocales[num_esferas];
    FuenteLuz flLocales[num_luces];
    cambiar_escena_locales(esfLocales,flLocales,T);

    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_real_distribution<> dist(0.0,1.0);
    for(int i=0 ; i<MAX_RAYOS ; i++){

        //Generar numero aleatorio para theta y phi [0,1)
        float th01 = dist(gen);
        float ph01 = dist(gen);

        //Calcular las muestras de theta y phi
        float theta = acos(sqrt(1-th01));
        float phi = 2*PI*ph01;

        //Lanzar rayo
        Vector omega_i = Vector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
        Rayo rIndirecto(interseccion, omega_i);

        int mas_cercana;           //Indice de la esfera mas cercana
        float punto_mas_cercano ; //Distancia a la que se encuentra el punto de interseccion
        colisionRayoObjetos(&rIndirecto, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo
        float intensidad[3] = {0.0, 0.0, 0.0};
        if (mas_cercana != -1) { //Si no ha intersectado con nada -> NEGRO
            //Se lanzan rayos secundarios
            Vector desplazamiento = Vector::productoEscalar(&omega_i, punto_mas_cercano);
            Punto sig_origen = Punto::desplazar(&interseccion, &desplazamiento);
            lanzar_secundarios(interseccion, sig_origen, desplazamiento.modulo(), mas_cercana, MAX_REBOTES, 0, intensidad);
        }
        //Dividir resultado entre la p(x) correspondiente
        float p_theta = 2*sin(theta)*cos(theta);
        float p_phi = 1.0 / (2*PI);
        intensidad[0] /= p_theta*p_phi;
        intensidad[1] /= p_theta*p_phi;
        intensidad[2] /= p_theta*p_phi;

        //Agregar al acumulado
        fr[0] += intensidad[0];
        fr[1] += intensidad[1];
        fr[2] += intensidad[2];
    }

    fr[0] /= MAX_RAYOS;
    fr[1] /= MAX_RAYOS;
    fr[2] /= MAX_RAYOS;
}
//Calcula el vector reflejado dado el vector de un rayo y la normal respecto a la que se quiere reflejar
Vector calcular_reflejado(Vector* rayo, Vector* normal){
    float omega_i_normal = Vector::pEscalar(rayo,normal);
    Vector normal_previo_normal = Vector::productoEscalar(normal,omega_i_normal);
    Vector menos_normal_previo_normal = Vector::productoEscalar(&normal_previo_normal,-1);
    Vector previo_mas_anterior = Vector::sumar(rayo,&menos_normal_previo_normal);
    Vector menos_dos_anterior = Vector::productoEscalar(&previo_mas_anterior, -2);
    return Vector::sumar(rayo,&menos_dos_anterior);
}
/**
 * Devuelve el vector reflejado rayo respecto a la normal dados los indices de refraccion n1 y n2
 * Copiado de un documento de la universidad de Standford
 */
Vector calcular_refractado(Vector* rayo, Vector* normal, double n1, double n2){
    Vector incidente = Vector::productoEscalar(rayo, -1);
    double n = n1/n2;
    double cosI = - Vector::pEscalar(normal, &incidente);
    double sinT2 = n*n*(1.0 - cosI*cosI);
    /**if(sinT2 > 1.0){
        return null;
    }*/
    double cosT = sqrt(1.0 - sinT2);
    Vector v1 = Vector::productoEscalar(&incidente,n);
    Vector v2 = Vector::productoEscalar(normal,(n*cosI - cosT));
    Vector refractado = Vector::sumar(&v1,&v2);
    return refractado;
}
//Calcula la iluminacion directa que llega a un punto(rayos de luz/sombra)
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum, float distancia){
    bool directa = true;                                   //Indice de la esfera mas cercana

    //Se calculan las intersecciones con las esferas
    for (int j=0 ; j<num_esferas && directa ; j++) {
        float soluciones[2];
        lista_esferas[j]->intersectar(r, soluciones);

        if (isfinite(soluciones[0]) && soluciones[0]>EPSILON && soluciones[0]<distancia) {
            directa = false;
        }
        if (isfinite(soluciones[1]) && soluciones[1]>EPSILON && soluciones[0]<distancia) {
            directa = false;
        }
    }

    if(directa) {
        float int_relativa = lista_luces[num_luz]->getEnergia() / ((dist_acum+distancia)*(dist_acum+distancia));
        if (int_relativa > 0.0) { // Si llega suficiente luz
            return int_relativa;
        }
    }
    return 0.0;
}
/**
 * Devuelve el indice de la esfera de la escena con la que colosiona el rayo
 * @param r
 * @return
 */
void colisionRayoObjetos(Rayo* r, int* i, float* j){
    int mas_cercana = -1;           //Indice de la esfera mas cercana
    float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion

    for (int k = 0; k < num_esferas; k++) {
        float soluciones[2];
        lista_esferas[k]->intersectar(r, soluciones);

        if (isfinite(soluciones[0]) && soluciones[0] > 0 && soluciones[0] < punto_mas_cercano && soluciones[0] > EPSILON) {
            mas_cercana = k;
            punto_mas_cercano = soluciones[0];
        }
        if (isfinite(soluciones[1]) && soluciones[1] > 0 && soluciones[1] < punto_mas_cercano && soluciones[1] > EPSILON) {
            mas_cercana = k;
            punto_mas_cercano = soluciones[1];
        }
    }
    *i = mas_cercana;
    *j = punto_mas_cercano;
}

void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotesInderectos, float* intensidad){
    Vector rayo_previo = Vector::getDireccion(&interseccion, &previo);
    rayo_previo.normalizar();

    //Se calcula la normal
    Punto centro_ultima = lista_esferas[ultima]->getOrigen();
    Vector normal = Vector::getDireccion(&centro_ultima,&interseccion);
    normal.normalizar();

    //Calculo del rayo reflejado mediante simetria
    Vector omega_r = calcular_reflejado(&rayo_previo,&normal);
    omega_r.normalizar();

    for(int i=0 ; i<num_luces ; i++){

        //Se calcula el rayo a la fuente de luz
        Punto punto_luz = lista_luces[i]->getOrigen();
        Vector omega_i = Vector::getDireccion(&interseccion, &punto_luz);
        float distancia = omega_i.modulo();
        omega_i.normalizar();
        Rayo rayo_luz(interseccion, omega_i);

        //Se lanza el rayo a la luz
        float li = lanzar_rayo_luz(&rayo_luz, i, dist_acum, distancia);

        //Calculamos el valor de la brdf
        float fr[3] = { 0.0, 0.0, 0.0 };
        brdf(&omega_i,&omega_r,ultima,fr);

        //Calculamos el coseno
        float cos_theta_i = Vector::cosenoVector(&normal, &omega_i);
        if(cos_theta_i<0){
            cos_theta_i = -cos_theta_i;
        }
        if(li>0 & i==1){
            i = 5;
        }
        //Se añade la luz al total
        intensidad[0] = intensidad[0] + fr[0]*li*cos_theta_i;
        intensidad[1] = intensidad[1] + fr[1]*li*cos_theta_i;
        intensidad[2] = intensidad[2] + fr[2]*li*cos_theta_i;
    }

    if(rebotesInderectos>0){
        float luzIndirecta[3] = { 0.0, 0.0, 0.0 };
        iluminacion_indirecta(interseccion,normal,luzIndirecta);
        intensidad[0] = intensidad[0] + luzIndirecta[0];
        intensidad[1] = intensidad[1] + luzIndirecta[1];
        intensidad[2] = intensidad[2] + luzIndirecta[2];
    }

}
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad){
    Vector rayo_previo = Vector::getDireccion(&interseccion, &previo);
    rayo_previo.normalizar();

    //Se calcula la normal
    Punto centro_ultima = lista_esferas[ultima]->getOrigen();
    Vector normal = Vector::getDireccion(&centro_ultima,&interseccion);
    normal.normalizar();

    //Calculo del rayo reflejado mediante simetria
    Vector omega_r = calcular_reflejado(&rayo_previo,&normal);
    omega_r.normalizar();

    Rayo reflejado(interseccion, omega_r);
    int mas_cercana;           //Indice de la esfera mas cercana
    float punto_mas_cercano ; //Distancia a la que se encuentra el punto de interseccion
    colisionRayoObjetos(&reflejado, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo

    if (mas_cercana != -1) { //Si no ha intersectado con nada -> NEGRO
        //Se lanzan rayos secundarios
        Vector desplazamiento = Vector::productoEscalar(&omega_r, punto_mas_cercano);
        Punto sig_origen = Punto::desplazar(&interseccion, &desplazamiento);
        lanzar_secundarios(interseccion, sig_origen, desplazamiento.modulo() + dist_acum, mas_cercana, rebotes-1, MAX_REBOTES_IND, intensidad);
    }
}
void fRefraccion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad){
    Vector rayo_previo = Vector::getDireccion(&interseccion, &previo);
    rayo_previo.normalizar();

    //Se calcula la normal
    Punto centro_ultima = lista_esferas[ultima]->getOrigen();
    Vector normal = Vector::getDireccion(&centro_ultima,&interseccion);

    float refraccionEntrada = 1;
    float refraccionSalida =  lista_esferas[ultima]->getKr();
    if(Vector::cosenoVector(&rayo_previo,&normal)<0){
        normal =  Vector(-normal.getX(), -normal.getY(), -normal.getZ());
        refraccionEntrada = refraccionSalida;
        refraccionSalida = 1;
    }
    normal.normalizar();

    //Calculo del rayo reflejado mediante simetria
    Vector omega_r = calcular_refractado(&rayo_previo,&normal, refraccionEntrada, refraccionSalida);
    omega_r.normalizar();
    Rayo reflejado(interseccion, omega_r);
    int mas_cercana;           //Indice de la esfera mas cercana
    float punto_mas_cercano ; //Distancia a la que se encuentra el punto de interseccion
    colisionRayoObjetos(&reflejado, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo

    if (mas_cercana != -1) { //Si no ha intersectado con nada -> NEGRO
        //Se lanzan rayos secundarios
        Vector desplazamiento = Vector::productoEscalar(&omega_r, punto_mas_cercano);
        Punto sig_origen = Punto::desplazar(&interseccion, &desplazamiento);
        lanzar_secundarios(interseccion, sig_origen, desplazamiento.modulo() + dist_acum, mas_cercana, rebotes-1, MAX_REBOTES_IND, intensidad);
    }
}
//Lanza los rayos secundarios(refractados, luz directa...) y guarda en luz los valores obtenidos
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad){
    if(rebotes != 0) {
        switch (lista_esferas[ultima]->getSuperficie()) {
            case Phong:
                fPhong(previo, interseccion, dist_acum, ultima, rebotesIndirectos, intensidad);
                break;
            case Reflexion:
                fReflexion(previo, interseccion, dist_acum, ultima, rebotes, intensidad);
                break;
            case Refraccion:

                fRefraccion(previo, interseccion, dist_acum, ultima, rebotes, intensidad);
                break;
        }
    }
}


int main() {
    //Creamos el fichero en el que guardar la vision de la escena
    ofstream fs("Prueba.ppm");
    fs << "P3" << endl << ANCHO_IMAGEN << " " << ALTO_IMAGEN << endl << COLOR_IMAGEN << endl;
    double i=ALTO_PANTALLA/2.0;
    for (int ind = 0 ; ind<ALTO_IMAGEN ; ind++) {
        i=i-TAM_PIXEL;
        double j=-ANCHO_PANTALLA/2.0 ;
        for (int ind2 = 0; ind2<ANCHO_IMAGEN ; ind2++) {
            j=j+TAM_PIXEL;

            if(i < ALTO_PANTALLA/2.0 - TAM_PIXEL*200){
                i=i+0;
            }

            // Calculamos la direccion al centro(+TAM_PIXEL/2) pixel correspondiente
            Vector d(TAM_PIXEL/2 + (float)j, TAM_PIXEL/2 + (float)i, DISTANCIA_PANTALLA);
            d.normalizar();

            //Creamos el rayo
            Rayo r(camara.getP(), d);

            int mas_cercana = -1;           //Indice de la esfera mas cercana
            float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion
            colisionRayoObjetos(&r, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo

            if (mas_cercana == -1) { //Si no ha intersectado con nada -> NEGRO
                fs << "0 0 0  ";
            } else {
                //Se lanzan rayos secundarios
                Vector desplazamiento = Vector::productoEscalar(&d, punto_mas_cercano);
                Punto sig_origen = Punto::desplazar(camara.getPref(), &desplazamiento);
                float intensidad[3] = {0.0, 0.0, 0.0};
                lanzar_secundarios(camara.getP(), sig_origen, desplazamiento.modulo(), mas_cercana, MAX_REBOTES, MAX_REBOTES_IND, intensidad);

                for (int k = 0; k < num_esferas; k++) {
                    if (mas_cercana == k) {

                        if(intensidad[0]>255){
                            intensidad[0]=255;
                        }
                        if(intensidad[1]>255){
                            intensidad[1]=255;
                        }
                        if(intensidad[2]>255){
                            intensidad[2]=255;
                        }

                        fs << (int) intensidad[0] << " "
                           << (int) intensidad[1] << " "
                           << (int) intensidad[2] << "  ";
                    }

                    fs.flush();
                }
            }
        }
        fs << endl;
    }
    fs.close();
    return 0;
}

Matriz calcular_locales(Vector normal, Punto posicion){
    Vector cualquiera = Vector(1,0,0);
    Vector u = Vector::pVectorial(&normal,&cualquiera);
    if(u.modulo()==0){
        cualquiera = Vector(0,1,0);
        u = Vector::pVectorial(&normal,&cualquiera);
        if(u.modulo()==0){
            cualquiera = Vector(0,0,1);
            u = Vector::pVectorial(&normal,&cualquiera);
        }
    }
    Vector v = Vector::pVectorial(&normal,&u);

    return  Matriz(u, v, normal, posicion);
}

Esfera cambiar_escena_locales(Esfera esferasLocales[], FuenteLuz lucesLocales[], Matriz T){
    for(int i = 0; i < num_esferas; i++){
        Esfera e = lista_esferas[i]->transformar(T);
        esferasLocales[i] = e;
    }

    for(int i = 0; i < num_luces; i++){
        FuenteLuz l = lista_luces[i]->transformar(T);
        lucesLocales[i] = l;
    }
}
