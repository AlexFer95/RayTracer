#include <iostream>
#include <fstream>
#include <cmath>
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
    double n = n1/n2;
    double cosI = - Vector::pEscalar(normal, rayo);
    double sinT2 = n*n*(1.0 - cosI*cosI);
    /**if(sinT2 > 1.0){
        return null;
    }*/
    double cosT = sqrt(1.0 - sinT2);
    Vector v1 = Vector::productoEscalar(rayo,n);
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

void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, float* intensidad){
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
        lanzar_secundarios(interseccion, sig_origen, desplazamiento.modulo() + dist_acum, mas_cercana, rebotes-1, intensidad);
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
        lanzar_secundarios(interseccion, sig_origen, desplazamiento.modulo() + dist_acum, mas_cercana, rebotes-1, intensidad);
    }
}
//Lanza los rayos secundarios(refractados, luz directa...) y guarda en luz los valores obtenidos
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad){
    if(rebotes != 0) {
        switch (lista_esferas[ultima]->getSuperficie()) {
            case Phong:
                fPhong(previo, interseccion, dist_acum, ultima, intensidad);
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

    for (double i=ALTO_PANTALLA/2.0 ; i>-ALTO_PANTALLA/2.0 ; i=i-TAM_PIXEL) {
        for (double j=-ANCHO_PANTALLA/2.0 ; j<ANCHO_PANTALLA/2.0 ; j=j+TAM_PIXEL) {

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
                lanzar_secundarios(camara.getP(), sig_origen, desplazamiento.modulo(), mas_cercana, MAX_REBOTES, intensidad);

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
                }
            }
        }
        fs << endl;
    }
    fs.close();
    return 0;
}