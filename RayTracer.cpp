#include <iostream>
#include <fstream>
#include <cmath>
#include "Punto.hpp"
#include "Vector.hpp"
#include "Matriz.hpp"
#include "Objetos.hpp"

using namespace std;

#define PI 3.14159

//Prototipos de las funciones
float lanzar_rayo_luz(Rayo* r, FuenteLuz* luz, Esfera* lista_esferas[], int num_esferas, float dist_acum);
void lanzar_rayo_reflejado(Rayo* r, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum, int rebotes, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum, int ultima, int rebotes, float* intensidad);


const int MAX_REBOTES = 1;   //Numero maximo de rebotes directos
const float EPSILON = 0.1;   //Distancia minima que debe recorrer el rayo
const float ANCHO = 3.4641;     //Ancho del plano pantalla (cuadrado)
const float ALTO = ANCHO;    //Alto del plano pantalla (cuadrado)
float DIST_PANTALLA;         //Distancia entre la camara y el plano
const float TAM_PIXEL = ANCHO / 400; //Unidades/Pixel
const int MAX_COLOR = 255;   //Color maximo permitido en la imagen




/**float lanzar_secundarios(Punto origen, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, int rebotes, float dist_acum, int ultima){

    float intensidad = 0; //Intensidad acumulada

    for(int i=0 ; i<num_luces ; i++){

        //Se calcula el rayo
        Punto punto_luz = lista_luces[i]->getOrigen();
        Vector d = Vector::getDireccion(&origen,&punto_luz);
        float distancia = d.modulo(); // Distancia directa a la fuente de luz
        d.normalizar();
        Rayo r(origen,d);

        int mas_cercana = -1;           //Indice de la esfera mas cercana
        float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion

        //Se calculan las intersecciones con las esferas
        for(int j=0 ; j<num_esferas ; j++){
            float soluciones[2];
            lista_esferas[j]->intersectar(&r,soluciones);

            if(isfinite(soluciones[0]) && soluciones[0]>EPSILON && soluciones[0]<punto_mas_cercano){
                mas_cercana=j;
                punto_mas_cercano=soluciones[0];
            }
            if(isfinite(soluciones[1]) && soluciones[1]>EPSILON && soluciones[1]<punto_mas_cercano) {
                mas_cercana = j;
                punto_mas_cercano = soluciones[1];
            }
        }

        if(mas_cercana==-1){ // No hay interseccion -> LLega la luz directamente
            float int_relativa = lista_luces[i]->getEnergia()/((dist_acum + distancia)*(dist_acum + distancia));
            Punto centro_ultima = lista_esferas[ultima]->getOrigen();
            Vector centro_origen = Vector::getDireccion(&centro_ultima,&origen);
            int_relativa = int_relativa * Vector::cosenoVector(&d,&centro_origen);
            if(int_relativa>0.0){ // Si llega suficiente luz
                intensidad = intensidad + int_relativa;
            }
        }
        else{
            if(rebotes<MAX_REBOTES){
                //Se calcula el siguiente origen
                Vector desplazamiento = Vector::productoEscalar(&d,punto_mas_cercano);
                Punto sig_origen = Punto::desplazar(&origen,&desplazamiento);

                //Se mira si se esta dentro de la esfera o se ha intersectado con sigo misma
                Punto centro_ultima_esfera = lista_esferas[ultima]->getOrigen();
                Vector direccion_centro_ultima_esfera = Vector::getDireccion(&centro_ultima_esfera,&sig_origen);
                if(mas_cercana!=ultima && direccion_centro_ultima_esfera.modulo()>=lista_esferas[ultima]->getRadio()){
                    intensidad = intensidad + lanzar_secundarios(sig_origen,lista_luces,num_luces,lista_esferas,num_esferas,rebotes+1,dist_acum+desplazamiento.modulo(),mas_cercana);
                }
            }
        }
    }

    return intensidad;
}
**/

//Calcula la iluminacion directa que llega a un punto
float lanzar_rayo_luz(Rayo* r, FuenteLuz* luz, Esfera* lista_esferas[], int num_esferas, float dist_acum){
    bool directa = true;                                   //Indice de la esfera mas cercana

    //Se calculan las intersecciones con las esferas
    for (int j=0 ; j<num_esferas && directa ; j++) {
        float soluciones[2];
        lista_esferas[j]->intersectar(r, soluciones);

        if (isfinite(soluciones[0]) && soluciones[0]>EPSILON) {
            directa = false;
        }
        if (isfinite(soluciones[1]) && soluciones[1]>EPSILON) {
            directa = false;
        }
    }

    if(directa) {
        float int_relativa = luz->getEnergia() / (dist_acum*dist_acum);
        if (int_relativa > 0.0) { // Si llega suficiente luz
            return int_relativa;
        }
    }
    return 0.0;
}

//Lanza los rayos refractados y guarda en luz los valores obtenidos
void lanzar_rayo_reflejado(Rayo* r, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum, int rebotes, float* intensidad){

    //Se calculan las intersecciones con las esferas
    int mas_cercana = -1;           //Indice de la esfera mas cercana
    float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion

    for(int j=0 ; j<num_esferas ; j++){
        float soluciones[2];
        lista_esferas[j]->intersectar(r,soluciones);

        if(isfinite(soluciones[0]) && soluciones[0]>EPSILON && soluciones[0]<punto_mas_cercano){
            mas_cercana=j;
            punto_mas_cercano=soluciones[0];
        }
        if(isfinite(soluciones[1]) && soluciones[1]>EPSILON && soluciones[1]<punto_mas_cercano) {
            mas_cercana = j;
            punto_mas_cercano = soluciones[1];
        }
    }

    if(mas_cercana!=-1){ // Hay alguna interseccion con las esferas
        if(rebotes<MAX_REBOTES){
            //Punto de la nueva interseccion
            Vector direccion_rayo = r->getDireccion();
            Punto origen_rayo = r->getOrigen();
            Vector desplazamiento = Vector::productoEscalar(&direccion_rayo,punto_mas_cercano);
            Punto sig_origen = Punto::desplazar(&origen_rayo,&desplazamiento);
            lanzar_secundarios(origen_rayo, sig_origen, lista_luces, num_luces, lista_esferas, num_esferas, dist_acum+desplazamiento.modulo(), mas_cercana, rebotes+1, intensidad);
        }
    }
}


//Lanza los rayos secundarios(refractados, luz directa...) y guarda en luz los valores obtenidos
void lanzar_secundarios(Punto previo, Punto interseccion, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum, int ultima, int rebotes, float* intensidad){
    Vector rayo_previo = Vector::getDireccion(&previo, &interseccion);

    for(int i=0 ; i<num_luces ; i++){

        //Se calcula el rayo a la fuente de luz
        Punto punto_luz = lista_luces[i]->getOrigen();
        Vector omega_i = Vector::getDireccion(&interseccion, &punto_luz);
        Vector omega_i_normalizado = omega_i;
        omega_i_normalizado.normalizar();
        float distancia = omega_i.modulo();
        Rayo rayo_luz(interseccion, omega_i_normalizado);
        //Se lanza el rayo a la luz
        float li = lanzar_rayo_luz(&rayo_luz, lista_luces[i], lista_esferas, num_esferas, dist_acum+distancia);

        //Se calcula la normal
        Punto centro_ultima = lista_esferas[ultima]->getOrigen();
        Vector normal = Vector::getDireccion(&centro_ultima,&interseccion);

        //Calculo del rayo reflejado mediante simetria
        /*float coseno = Vector::cosenoVector(&omega_i_normalizado,&normal);
        Vector rayo_proyeccion = Vector::productoEscalar(&normal, distancia*coseno);
        Punto origen_proyeccion = Punto::desplazar(&interseccion, &rayo_proyeccion);
        Vector origen_proyeccion_normal = Vector::getDireccion(&punto_luz, &origen_proyeccion);
        origen_proyeccion_normal = Vector::productoEscalar(&origen_proyeccion_normal,2.0);
        Punto reflejo = Punto::desplazar(&punto_luz, &origen_proyeccion_normal);
        Vector vector_reflejado = Vector::getDireccion(&interseccion, &reflejo);
        vector_reflejado.normalizar();*/
        float omega_i_normal = Vector::pEscalar(&omega_i,&normal);
        Vector normal_omega_i_normal = Vector::productoEscalar(&normal,omega_i_normal);
        Vector menos_normal_omega_i_normal = Vector::productoEscalar(&normal_omega_i_normal,-1);
        Vector omega_i_mas_anterior = Vector::sumar(&omega_i,&menos_normal_omega_i_normal);
        Vector menos_dos_anterior = Vector::productoEscalar(&omega_i_mas_anterior, -2);
        Vector omega_r = Vector::sumar(&omega_i,&menos_dos_anterior);

        //Calculo de fr
        float fr[3] = { 0.0, 0.0, 0.0 };

        float cos_theta_r =  Vector::cosenoVector(&normal,&omega_r);
        if(cos_theta_r<0){
            cos_theta_r = -cos_theta_r;
        }



        float ks = lista_esferas[ultima]->getKs()*((2+10)/(2*PI))*pow(cos_theta_r,10); // Alpha = 5 constante
        fr[0] = lista_esferas[ultima]->getKd()[0]/PI + ks;
        fr[1] = lista_esferas[ultima]->getKd()[1]/PI + ks;
        fr[2] = lista_esferas[ultima]->getKd()[2]/PI + ks;

        float cos_theta_i = Vector::cosenoVector(&normal, &omega_i);
        if(cos_theta_i<0){
            cos_theta_i = -cos_theta_i;
        }

        intensidad[0] = intensidad[0] + fr[0]*li*cos_theta_i;
        intensidad[1] = intensidad[1] + fr[1]*li*cos_theta_i;
        intensidad[2] = intensidad[2] + fr[2]*li*cos_theta_i;
    }
}

int main() {
    //Origen del sistema
    Punto origen(0, 0, 0);

    //Calculamos la distancia a la que se coloca la pantalla
    DIST_PANTALLA = (float) tan(30) * ANCHO / 2;

    //Definir camara (matriz)
    Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO / 2, ALTO / 2, 0));

    //Definir fuentes de luz
    FuenteLuz f0(Punto(ANCHO/2+1, ANCHO/2, DIST_PANTALLA*1), 9000000000);
    int num_luces = 1;
    FuenteLuz *lista_luces[] = {&f0};

    //Definir objetos de la escena
    Esfera e0(Punto(camara.getPref()->getX(), camara.getPref()->getY(), DIST_PANTALLA*5), 5, 0.5, 0.5, 0.5, 0.8);
    int num_esferas = 1;
    Esfera *lista_esferas[] = {&e0};

    //Creamos el fichero en el que guardar la vision de la escena
    ofstream fs("Prueba.ppm");
    fs << "P3" << endl << ANCHO / TAM_PIXEL +1 << " " << ALTO / TAM_PIXEL + 1 << endl << MAX_COLOR << endl;

    for (double i = ALTO / 2.0 ; i > -ALTO / 2.0 ; i=i-TAM_PIXEL) {
        for (double j = -ANCHO / 2.0 ; j < ANCHO / 2.0 ; j=j+TAM_PIXEL) {

            // Calculamos la direccion al centro(+TAM_PIXEL/2) pixel correspondiente
            Vector d(TAM_PIXEL / 2 + (float) j, TAM_PIXEL / 2 + (float) i, DIST_PANTALLA);
            //cout << "(" << d.getX() << "," << d.getY() << "," << d.getZ() << ")" << endl;
            d.normalizar();

            //Creamos el rayo
            Rayo r(camara.getP(), d);

            int mas_cercana = -1;           //Indice de la esfera mas cercana
            float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion

            //cout << "hola1" << endl;

            for (int k = 0; k < num_esferas; k++) {
                float soluciones[2];
                lista_esferas[k]->intersectar(&r, soluciones);

                if (isfinite(soluciones[0]) && soluciones[0] > 0 && soluciones[0] < punto_mas_cercano) {
                    mas_cercana = k;
                    punto_mas_cercano = soluciones[0];
                }
                if (isfinite(soluciones[1]) && soluciones[1] > 0 && soluciones[1] < punto_mas_cercano) {
                    mas_cercana = k;
                    punto_mas_cercano = soluciones[1];
                }
            }

            //cout << "hola2" << endl;

            if (mas_cercana == -1) { //Si no ha intersectado con nada -> NEGRO
                fs << "0 0 0  ";
            } else {
                //Se lanzan rayos secundarios
                Vector desplazamiento = Vector::productoEscalar(&d, punto_mas_cercano);
                Punto sig_origen = Punto::desplazar(camara.getPref(), &desplazamiento);
                float intensidad[3] = {0.0, 0.0, 0.0};
                lanzar_secundarios(camara.getP(), sig_origen, lista_luces, num_luces, lista_esferas, num_esferas,
                                   desplazamiento.modulo(), mas_cercana, 0, intensidad);

                for (int k = 0; k < num_esferas; k++) {
                    if (mas_cercana == k) {
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