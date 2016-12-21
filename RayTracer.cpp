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

//Carga la escena elegida
void cargar_escena(int escena){
    switch(escena){
        case 1:
            num_luces = 1;
            num_esferas = 7;
            lista_esferas[0] = &cv_left;
            lista_esferas[1] = &cv_right;
            lista_esferas[2] = &cv_floor;
            lista_esferas[3] = &cv_roof;
            lista_esferas[4] = &cv_back;
            lista_esferas[5] = &cv_spe;
            lista_esferas[6] = &cv_ref;
            lista_luces[0] = &f0;
            break;
        case 2:
            num_luces = 2;
            num_esferas = 12;
            lista_esferas[0] = &cv_left2;

            lista_esferas[1] = &tronco1;
            lista_esferas[2] = &tronco2;
            lista_esferas[3] = &hojas1;
            lista_esferas[4] = &hojas2;
            lista_esferas[5] = &hojas3;
            lista_esferas[6] = &bola1;
            lista_esferas[7] = &bola2;
            lista_esferas[8] = &bola3;
            lista_esferas[9] = &bola4;
            lista_esferas[10] = &bola5;

            lista_esferas[11] = &cv_floor2;
           /* lista_esferas[1] = &cv_right2;
            lista_esferas[2] = &cv_floor2;
            lista_esferas[3] = &cv_roof2;
            lista_esferas[4] = &cv_back2;
            lista_esferas[5] = &tronco1;
            lista_esferas[6] = &tronco2;
            lista_esferas[7] = &hojas1;
            lista_esferas[8] = &hojas2;
            lista_esferas[9] = &hojas3;
            lista_esferas[10] = &bola1;
            lista_esferas[11] = &bola2;
            lista_esferas[12] = &bola3;
            lista_esferas[13] = &bola4;
            lista_esferas[14] = &bola5;*/
            lista_luces[0] = &f0_2;
            lista_luces[1] = &f1_2;
            break;
        case 3:
            num_luces = 2;
            num_esferas = 5;

            lista_esferas[0] = &suelo1;
            lista_esferas[1] = &suelo2;
            lista_esferas[2] = &suelo3;
            lista_esferas[3] = &suelo4;
            lista_esferas[4] = &e0_3;
            //lista_esferas[5] = &e2;

            lista_luces[0] = &f0_3;
            lista_luces[1] = &f1_3;
            break;
        case 4:

            num_luces = 2;
            num_esferas = 20;

            lista_esferas[0] = &e0_4;
            lista_esferas[1] = &e1_4;
            lista_esferas[2] = &e2_4;
            lista_esferas[3] = &e3_4;
            lista_esferas[4] = &e4_4;

            lista_esferas[5] = &e5_4;
            lista_esferas[6] = &e6_4;
            lista_esferas[7] = &e7_4;
            lista_esferas[8] = &e8_4;
            lista_esferas[9] = &e9_4;

            lista_esferas[10] = &e10_4;
            lista_esferas[11] = &e11_4;
            lista_esferas[12] = &e12_4;
            lista_esferas[13] = &e13_4;
            lista_esferas[14] = &e14_4;

            lista_esferas[15] = &e15_4;
            lista_esferas[16] = &e16_4;
            lista_esferas[17] = &e17_4;
            lista_esferas[18] = &e18_4;
            lista_esferas[19] = &e19_4;



            lista_luces[0] = &f0_4;
            lista_luces[1] = &f1_4;
            break;
    }
}

//Calcula la matriz de transformacion a coordenadas locales/globales dada la normal y un punto
Matriz calcular_locales(Vector normal, Punto posicion){
    Vector cualquiera = Vector(1,0,0);
    Vector u = Vector::pVectorial(&normal,&cualquiera);
    //Si el vector resultante no es perpendicular a la normal se prueba con otro
    if(u.modulo()==0){
        cualquiera = Vector(0,1,0);
        u = Vector::pVectorial(&normal,&cualquiera);

        //Si el vector resultante no es perpendicular a la normal se prueba con otro que por descarte lo sera
        if(u.modulo()==0){
            cualquiera = Vector(0,0,1);
            u = Vector::pVectorial(&normal,&cualquiera);
        }
    }
    u.normalizar();
    Vector v = Vector::pVectorial(&normal,&u);
    v.normalizar();

    return  Matriz(u, v, normal, posicion);
}

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

//Calcula la iluminación indirecta por montecarlo
void iluminacion_indirecta(Punto interseccion, Vector normal, float* fr, Vector omega_o, int esfera, int rebotes, int rebotesInd){

    Matriz T = calcular_locales(normal, interseccion);

    for(int i=0 ; i<MAX_RAYOS ; i++){

        float th01 = distribution(generator);
        float ph01 = distribution(generator);
        //Calcular las muestras de theta y phi
        float theta = acos(sqrt(1-th01));
        float phi = 2*PI*ph01;

        //Lanzar rayo
        Vector v = Vector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
        Vector omega_i = T.transformar(&v);
        omega_i.normalizar();
        Rayo rIndirecto(interseccion, omega_i);

        int mas_cercana;           //Indice de la esfera mas cercana
        float punto_mas_cercano; //Distancia a la que se encuentra el punto de interseccion
        colisionRayoObjetos(&rIndirecto, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo
        float intensidad[3] = {0.0, 0.0, 0.0};
        if (mas_cercana != -1) { //Si no ha intersectado con nada -> NEGRO
            //Se lanzan rayos secundarios
            Vector desplazamiento = Vector::productoEscalar(&omega_i, punto_mas_cercano);
            Punto sig_origen = Punto::desplazar(&interseccion, &desplazamiento);
            lanzar_rayos(interseccion, sig_origen, mas_cercana, rebotes, --rebotesInd, intensidad);
        }
        float intensidad_brdf[3] = {0.0, 0.0, 0.0};
        brdf(&omega_i, &omega_o, esfera, intensidad_brdf);
        float valor = cos(theta)*sin(theta);
        intensidad[0] *= intensidad_brdf[0]*valor;
        intensidad[1] *= intensidad_brdf[1]*valor;
        intensidad[2] *= intensidad_brdf[2]*valor;

        //Dividir resultado entre la p(x) correspondiente
        float p_theta = 2.0*sin(theta)*cos(theta);
        float p_phi = 1.0 / (2.0*PI);
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

//Devuelve el vector reflejado rayo respecto a la normal dados los indices de refraccion n1 y n2
//Basado en un documento de la universidad de Standford
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
float lanzar_rayo_luz(Rayo* r, int num_luz, float distancia, bool indirecta){
    bool directa = true;

    //Se calculan las intersecciones con las esferas
    for (int j=0 ; j<num_esferas && directa ; j++) {
        float soluciones[2];
        lista_esferas[j]->intersectar(r, soluciones);

        if (isfinite(soluciones[0]) && soluciones[0]<distancia && soluciones[0]>EPSILON) {
            directa = false;
        }
        if (isfinite(soluciones[1]) && soluciones[1]<distancia && soluciones[1]>EPSILON) {
            directa = false;
        }
    }

    if(directa && indirecta) {
        return lista_luces[num_luz]->getEnergia();
    }
    if(directa){
        return  lista_luces[num_luz]->getEnergia() / distancia*distancia;
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

        if (isfinite(soluciones[0]) && soluciones[0] < punto_mas_cercano && soluciones[0] > EPSILON) {
            mas_cercana = k;
            punto_mas_cercano = soluciones[0];
        }
        if (isfinite(soluciones[1]) && soluciones[1] < punto_mas_cercano && soluciones[1] > EPSILON) {
            mas_cercana = k;
            punto_mas_cercano = soluciones[1];
        }
    }
    *i = mas_cercana;
    *j = punto_mas_cercano;
}

void fPhong(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad){
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
        float li = lanzar_rayo_luz(&rayo_luz, i, distancia, rebotesIndirectos>0);

        //Calculamos el valor de la brdf
        float fr[3] = { 0.0, 0.0, 0.0 };
        brdf(&omega_i,&omega_r,ultima,fr);

        //Calculamos el coseno
        float cos_theta_i = Vector::cosenoVector(&normal, &omega_i);
        cos_theta_i = Vector::pEscalar(&normal, &omega_i);
        if(cos_theta_i<0){
            cos_theta_i = -cos_theta_i;
        }
        //Se añade la luz al total
        intensidad[0] = intensidad[0] + fr[0]*li*cos_theta_i;
        intensidad[1] = intensidad[1] + fr[1]*li*cos_theta_i;
        intensidad[2] = intensidad[2] + fr[2]*li*cos_theta_i;
    }

    if(rebotesIndirectos>0){
        float luzIndirecta[3] = { 0.0, 0.0, 0.0 };
        iluminacion_indirecta(interseccion, normal, luzIndirecta, omega_r, ultima, rebotes, rebotesIndirectos);
        intensidad[0] = intensidad[0] + luzIndirecta[0];//*0.1;
        intensidad[1] = intensidad[1] + luzIndirecta[1];//*0.1;
        intensidad[2] = intensidad[2] + luzIndirecta[2];//*0.1;
    }

}
void fReflexion(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad){
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
        lanzar_rayos(interseccion, sig_origen, mas_cercana, rebotes-1, rebotesIndirectos, intensidad);
    }
}
void fRefraccion(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad){
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
        lanzar_rayos(interseccion, sig_origen, mas_cercana, rebotes-1, rebotesIndirectos, intensidad);
    }
}
//Lanza los rayos (refractados, luz directa...) y guarda en luz los valores obtenidos
void lanzar_rayos(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad){
    if(rebotes != 0) {
        switch (lista_esferas[ultima]->getSuperficie()) {
            case Phong:
                fPhong(previo, interseccion, ultima, rebotes, rebotesIndirectos, intensidad);
                break;
            case Reflexion:
                fReflexion(previo, interseccion, ultima, rebotes, rebotesIndirectos, intensidad);
                break;
            case Refraccion:
                fRefraccion(previo, interseccion, ultima, rebotes, rebotesIndirectos, intensidad);
                break;
        }
    }
}


int main(int argc, char* argv[]) {

    char uso[] = "Uso: ./RayTracer [num_escena] [tam_imagen] [num_rayos_indirectos] [post_procesado]";

    if(argc != 5){
        cout << uso << endl;
        return -1;
    }

    int escena = atoi(argv[1]);
    if(escena<1 || escena>NUM_ESCENAS){
        cout << uso << endl;
        cout << "[num_escena] debe estar comprendido entre 1 y " << NUM_ESCENAS << endl;
        return -1;
    }

    ANCHO_IMAGEN = atoi(argv[2]);
    ALTO_IMAGEN = ANCHO_IMAGEN;
    TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;
    if(ANCHO_IMAGEN<100){
        cout << "Uso: ./RayTracer [num_escena] [tam_imagen] [num_rayos_indirectos]" << endl;
        cout << "[tam_imagen] debe ser mayor que 100" << endl;
        return -1;
    }

    MAX_RAYOS = atoi(argv[3]);
    if(MAX_RAYOS<1){
        cout << uso << endl;
        cout << "[num_rayos_indirectos] debe ser mayor que 1" << endl;
        return -1;
    }

    int post_procesado = atoi(argv[4]);
    if(post_procesado==0){
        POST_PROCESADO = false;
    }
    else if(post_procesado==1){
        POST_PROCESADO = true;
    }
    else{
        cout << uso << endl;
        cout << "[post_procesado] debe ser 0 para limitar a 255 el exceso de luz" << endl;
        cout << "[post_procesado] debe ser 1 para adecuar la imagen a la maxima energia" << endl;
        return -1;
    }

    cargar_escena(escena);

    //Creamos el buffer
    buffer = new float**[ANCHO_IMAGEN];
    for(int i=0 ; i<ANCHO_IMAGEN ; i++){
        buffer[i] = new float*[ALTO_IMAGEN];
        for(int j=0 ; j<ALTO_IMAGEN ; j++){
            buffer[i][j] = new float[3];
        }
    }

    //variable para ver porque pixel va el programa
    int nIteracion=0;
    int max_iteraciones = ANCHO_IMAGEN*ALTO_IMAGEN;

    float max_color = 0.0;
    double i=ALTO_PANTALLA/2.0;

    for (int ind = 0 ; ind<ALTO_IMAGEN ; ind++) {
        i=i-TAM_PIXEL;
        double j=-ANCHO_PANTALLA/2.0 ;
        for (int ind2 = 0; ind2<ANCHO_IMAGEN ; ind2++) {
            j=j+TAM_PIXEL;
            nIteracion++;

            // Calculamos la direccion al centro(+TAM_PIXEL/2) pixel correspondiente
            Vector d(TAM_PIXEL/2 + (float)j, TAM_PIXEL/2 + (float)i, DISTANCIA_PANTALLA);
            d.normalizar();

            //Creamos el rayo
            Rayo r(camara.getP(), d);

            int mas_cercana = -1;           //Indice de la esfera mas cercana
            float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion
            colisionRayoObjetos(&r, &mas_cercana, &punto_mas_cercano); // Esfera con la que colisiona el rayo

            if (mas_cercana == -1) { //Si no ha intersectado con nada -> NEGRO
                buffer[ind2][ind][0] = 0.0;
                buffer[ind2][ind][1] = 0.0;
                buffer[ind2][ind][2] = 0.0;
            } else {
                //Se lanzan rayos secundarios
                Vector desplazamiento = Vector::productoEscalar(&d, punto_mas_cercano);
                Punto sig_origen = Punto::desplazar(camara.getPref(), &desplazamiento);
                float intensidad[3] = {0.0,0.0,0.0};
                lanzar_rayos(camara.getP(), sig_origen, mas_cercana, MAX_REBOTES, MAX_REBOTES_IND, intensidad);

                if(intensidad[0]>max_color){
                    max_color = intensidad[0];
                }
                if(intensidad[1]>max_color){
                    max_color = intensidad[1];
                }
                if(intensidad[2]>max_color){
                    max_color = intensidad[2];
                }

                buffer[ind2][ind][0] = intensidad[0];
                buffer[ind2][ind][1] = intensidad[1];
                buffer[ind2][ind][2] = intensidad[2];
            }
        }

        cout << (float)nIteracion/(float)max_iteraciones * 100 <<"%\n";
    }

    //Escribimos la cabecera del fichero de salida
    ofstream fs("Prueba.ppm");
    fs << "P3" << endl << ANCHO_IMAGEN << " " << ALTO_IMAGEN << endl << COLOR_IMAGEN << endl;

    //Volcamos el buffer en el fichero
    for(int i=0 ; i<ALTO_IMAGEN ; i++){
        for(int j=0 ; j<ANCHO_IMAGEN ; j++){
            for(int k=0 ; k<3 ; k++){
                if(POST_PROCESADO) {
                    if (255 * buffer[j][i][k] > 255) {
                        fs << 255 << " ";
                    }
                    else {
                        fs << (int) (255 * buffer[j][i][k]) << " ";
                    }
                }
                else{
                        fs << (int) (255 * buffer[j][i][k] / max_color) << " ";
                }

            }
            fs << " ";
        }
        fs << endl;
    }
    fs.flush();
    fs.close();

    //Destruimos el buffer
    for(int i=0 ; i<ANCHO_IMAGEN ; i++){
        for(int j=0 ; j<ALTO_IMAGEN ; j++){
            delete[] buffer[i][j];
        }
        delete[] buffer[i];
    }

    return 0;
}


