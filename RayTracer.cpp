#include <iostream>
#include <fstream>
#include <cmath>
#include "Punto.hpp"
#include "Vector.hpp"
#include "Matriz.hpp"
#include "Objetos.hpp"

//Numero maximo de rebotes directos
const int MAX_REBOTES = 4;
//Distancia minima del rayo rebotado
const float EPSILON = 0.5;
//Ancho y alto del plano pantalla (cuadrado)
const float ANCHO = 400;
const float ALTO = ANCHO;
float DIST_PANTALLA;
// Unidades/Pixel
const float TAM_PIXEL = 1.0;
//Color maximo permitido en la imagen
const int MAX_COLOR = 255;

using namespace std;

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
float lanzar_rayos_sombra(Punto origen, Vector* normal, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum){
    float intensidad = 0.0;
    for(int i=0 ; i<num_luces ; i++) {
        //Se calcula el rayo
        Punto punto_luz = lista_luces[i]->getOrigen();
        Vector d = Vector::getDireccion(&origen, &punto_luz);
        float distancia = d.modulo();                           // Distancia directa a la fuente de luz
        d.normalizar();
        Rayo r(origen, d);

        bool directa = true;                                   //Indice de la esfera mas cercana

        //Se calculan las intersecciones con las esferas
        for (int j=0 ; j<num_esferas && directa ; j++) {
            float soluciones[2];
            lista_esferas[j]->intersectar(&r, soluciones);

            if (isfinite(soluciones[0]) && soluciones[0]>EPSILON && soluciones[0]<distancia) {
                directa = false;
            }
            if (isfinite(soluciones[1]) && soluciones[1]>EPSILON && soluciones[1]<distancia) {
                directa = false;
            }
        }

        if(directa) {
            float int_relativa = lista_luces[i]->getEnergia()/((dist_acum + distancia)*(dist_acum + distancia));
            int_relativa = int_relativa * Vector::cosenoVector(&d,normal);
            if(int_relativa>0.0){ // Si llega suficiente luz
                intensidad = intensidad + int_relativa;
            }
        }
    }
    return intensidad;
}

//Lanza los rayos refractados y guarda en luz los valores obtenidos
void lanzar_rayo_refractado(Punto origen, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, int rebotes, float dist_acum, int ultima, float* luz){

    //Valores para probar que funciona...
    luz[0] = 255;
    luz[1] = 255;
    luz[2] = 255;


    //Calcular rayo refractado

    //luz[x] = luz[x] en este punto + lanzar_rayo_refractado(siguiente punto, rebote)???
}


//Lanza los rayos secundarios(refractados, luz directa...) y guarda en luz los valores obtenidos
void lanzar_secundarios(Punto origen, FuenteLuz* lista_luces[], int num_luces, Esfera* lista_esferas[], int num_esferas, float dist_acum, int ultima, float* luz){

    lanzar_rayo_refractado(origen, lista_luces, num_luces, lista_esferas, num_esferas, 0, dist_acum, ultima, luz);

    //Calculamos la luz directa
    Punto centro_ultima = lista_esferas[ultima]->getOrigen();
    Vector normal = Vector::getDireccion(&centro_ultima,&origen);
    float directa = lanzar_rayos_sombra(origen, &normal, lista_luces, num_luces, lista_esferas, num_esferas, dist_acum);

    luz[0] = luz[0] * directa;
    luz[1] = luz[1] * directa;
    luz[2] = luz[2] * directa;
}

int main(){
    //Origen del sistema
    Punto origen(0,0,0);

    //Calculamos la distancia a la que se coloca la pantalla
    DIST_PANTALLA = (float) tan(30) * ANCHO/2;

    //Definir camara (matriz)
    Matriz camara(Vector(1,0,0),Vector(0,1,0),Vector(0,0,1),Punto(ANCHO/2,ALTO/2,0));

    //Definir fuentes de luz
    FuenteLuz f0(Punto(camara.getPref()->getX()-100,camara.getPref()->getY(),DIST_PANTALLA*4),700000);
    FuenteLuz f1(Punto(camara.getPref()->getX()-350,camara.getPref()->getY(),DIST_PANTALLA*1),700000);
    int num_luces = 2;
    float luz_total = 0.0;
    FuenteLuz* lista_luces[] = { &f0, &f1 };
    for(int i=0 ; i<num_luces ; i++){
        luz_total = luz_total + lista_luces[i]->getEnergia();
    }

    //Definir objetos de la escena
    Esfera e0(Punto(camara.getPref()->getX()+250,camara.getPref()->getY(),DIST_PANTALLA*3),100,100,100,0.0);
    Esfera e1(Punto(camara.getPref()->getX()-100,camara.getPref()->getY(),DIST_PANTALLA*2),100,0.0,100,0.0);
    //Esfera e2(Punto(camara.getPref()->getX(),camara.getPref()->getY(),DIST_PANTALLA*2),100,0.0,0.0,100);
    int num_esferas = 2;
    Esfera* lista_esferas[] = { &e0, &e1 };

    //Creamos el fichero en el que guardar la vision de la escena
    ofstream fs("Prueba.ppm");
    fs << "P3" << endl << ANCHO/TAM_PIXEL << " " << ALTO/TAM_PIXEL << endl << MAX_COLOR << endl;

    for(double i=ALTO/TAM_PIXEL/2.0 ; i>-ALTO/TAM_PIXEL/2.0 ; i--){
        for(double j=-ANCHO/TAM_PIXEL/2.0 ; j<ANCHO/TAM_PIXEL/2.0 ; j++){

            // Calculamos la direccion al centro(+TAM_PIXEL/2) pixel correspondiente
            Vector d(TAM_PIXEL/2+(float)j, TAM_PIXEL/2+(float)i, DIST_PANTALLA);
            //cout << "(" << d.getX() << "," << d.getY() << "," << d.getZ() << ")" << endl;
            d.normalizar();

            //Creamos el rayo
            Rayo r(camara.getP(),d);

            int mas_cercana = -1;           //Indice de la esfera mas cercana
            float punto_mas_cercano = INFINITY; //Distancia a la que se encuentra el punto de interseccion

            //cout << "hola1" << endl;

            for(int k=0 ; k<num_esferas ; k++){
                float soluciones[2];
                lista_esferas[k]->intersectar(&r, soluciones);

                if(isfinite(soluciones[0]) && soluciones[0]>0 && soluciones[0]<punto_mas_cercano){
                    mas_cercana = k;
                    punto_mas_cercano = soluciones[0];
                }
                if(isfinite(soluciones[1]) && soluciones[1]>0 && soluciones[1]<punto_mas_cercano){
                    mas_cercana = k;
                    punto_mas_cercano = soluciones[1];
                }
            }

            //cout << "hola2" << endl;

            if(mas_cercana==-1){ //Si no ha intersectado con nada -> NEGRO
                fs << "0 0 0  ";
            }
            else{
                //Se lanzan rayos secundarios
                Vector desplazamiento = Vector::productoEscalar(&d,punto_mas_cercano);
                Punto sig_origen = Punto::desplazar(camara.getPref(),&desplazamiento);
                float luz[3] = { 0.0, 0.0, 0.0 };
                lanzar_secundarios(sig_origen,lista_luces,num_luces,lista_esferas,num_esferas,0.0,mas_cercana,luz);

                for(int k=0 ; k<num_esferas ; k++){
                    if(mas_cercana==k){
                        fs << (int)luz[0] << " "
                           << (int)luz[1] << " "
                           << (int)luz[2] << "  ";
                    }
                }
            }
        }
        fs << endl;
    }
    fs.close();
    return 0;
}