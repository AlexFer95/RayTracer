
#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include "Punto.hpp"
#include "Vector.hpp"
#include "Matriz.hpp"
#include "Objetos.hpp"

#define PI 3.14159
#define ALPHA 70
#define MAX_REBOTES 4
#define MAX_REBOTES_IND 1
#define EPSILON 0.001
#define NUM_ESCENAS 4

//CONSTANTES DE LA IMAGEN Y LA PANTALLA
const float COLOR_IMAGEN = 255;
const float DISTANCIA_PANTALLA = 1;
const float ANCHO_PANTALLA = 1;
const float ALTO_PANTALLA = 1;


//VARIABLES DE LA IMAGEN Y LA PANTALLA
int ANCHO_IMAGEN;
int ALTO_IMAGEN;
float TAM_PIXEL;

//COLORES
const float amarillo[3] = { 0.9 , 0.84 , 0.07 };
const float azul[3] = { 0.2 , 0.4 , 0.4 };
const float azul2[3] = { 0.1 , 0.1 , 0.8 };
const float morado[3] = { 0.8 , 0.4 , 0.8 };
const float negro[3] = { 0.0 , 0.0 , 0.0 };
const float rojo[3] = { 0.8 , 0.1 , 0.1 };
const float verde[3] = { 0.1 , 0.8 , 0.1 };
const float marron[3] = { 0.24 , 0.12 , 0.09};
const float gris[3] = { 0.3333 , 0.3333 , 0.3333};
const float blanco[3] = {1.0 , 1.0 , 1.0};

//VARIABLES DEL TRAZADOR
int MAX_RAYOS;
bool POST_PROCESADO;
float*** buffer;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0,1.0);

FuenteLuz *lista_luces[20];
Esfera *lista_esferas[25];
int num_luces;
int num_esferas;

Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0));

//Definir materiales para las escenas
Material p_azul = { azul, 0.0 , 0.0 , Phong};
Material p_azul2 = { azul, 0.001 , 0.0 , Phong};
Material p_azul3 = { azul, 0.01 , 0.0 , Phong};
Material p_azul4 = { azul, 0.1 , 0.0 , Phong};
Material p_azul5 = { azul, 0.3 , 0.0 , Phong};
Material p_amar = { amarillo , 0.0 , 0.0 , Phong};
Material p_amar2 = { amarillo , 0.001 , 0.0 , Phong};
Material p_amar3 = { amarillo , 0.01 , 0.0 , Phong};
Material p_amar4 = { amarillo , 0.1 , 0.0 , Phong};
Material p_amar5 = { amarillo , 0.3 , 0.0 , Phong};
Material p_mora = { morado , 0.0 , 0.0 , Phong};
Material p_mora2 = { morado , 0.1 , 0.0 , Phong};
Material p_rojo = { rojo , 0.0 , 0.0 , Phong};
Material p_rojo2 = { rojo , 0.001 , 0.0 , Phong};
Material p_rojo3 = { rojo , 0.01, 0.0 , Phong};
Material p_rojo4 = { rojo , 0.1 , 0.0 , Phong};
Material p_rojo5 = { rojo , 0.3 , 0.0 , Phong};
Material p_verde = { verde , 0.0 , 0.0 , Phong};
Material p_verde2 = { verde , 0.001 , 0.0 , Phong};
Material p_verde3 = { verde , 0.01 , 0.0 , Phong};
Material p_verde4 = { verde , 0.1 , 0.0 , Phong};
Material p_verde5 = { verde , 0.3 , 0.0 , Phong};
Material p_marron = { marron , 0.0 , 0.0 , Phong};
Material p_gris = { gris , 0.0 , 0.0 , Phong};
Material p_blanco = { blanco , 0.0 , 0.0 , Phong};
Material p_blanco2 = { blanco , 0.1 , 0.0 , Phong};
Material reflex = { blanco , 0.0 , 0.0 , Reflexion};
Material refrac = { blanco , 0.0 , 1.7 , Refraccion};

//ESCENA 1: CORNELL BOX (1x1)
Esfera cv_left(Punto(camara.getPref()->getX() - 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_rojo);
Esfera cv_right(Punto(camara.getPref()->getX() + 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_verde);
Esfera cv_floor(Punto(camara.getPref()->getX() , camara.getPref()->getY() - 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_gris);
Esfera cv_roof(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_gris);
Esfera cv_back(Punto(camara.getPref()->getX() , camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 1001), 1000, p_gris);
Esfera cv_spe(Punto(camara.getPref()->getX() - 0.15, camara.getPref()->getY() - 0.25, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.7), 0.25, reflex);
Esfera cv_ref(Punto(camara.getPref()->getX() + 0.25, camara.getPref()->getY() - 0.35, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.4), 0.15, refrac);
FuenteLuz f0(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 0.2 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.3), 1);

//ESCENA 2: christmas tree
Esfera cv_left2(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 100, p_azul);
//Esfera cv_right2(Punto(camara.getPref()->getX() + 1002.5, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_azul);
Esfera cv_floor2(Punto(camara.getPref()->getX() , camara.getPref()->getY() - 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_gris);
//Esfera cv_roof2(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_azul);
//Esfera cv_back2(Punto(camara.getPref()->getX() , camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 1005), 1000, p_azul);
Esfera tronco1(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.5, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.1, p_marron);
Esfera tronco2(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.4, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.1, p_marron);
Esfera hojas1(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.2, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.2, p_verde);
Esfera hojas2(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.1, p_verde);
Esfera hojas3(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.1, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.05, p_verde);
Esfera bola1(Punto(camara.getPref()->getX()-0.1, camara.getPref()->getY()-0.2, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.19), 0.02, p_azul2);
Esfera bola2(Punto(camara.getPref()->getX()+0.1, camara.getPref()->getY()-0.1, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.02, p_rojo2);
Esfera bola3(Punto(camara.getPref()->getX()+0.05, camara.getPref()->getY(), camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.3), 0.02, p_mora2);
Esfera bola4(Punto(camara.getPref()->getX()-0.05, camara.getPref()->getY()+0.1, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.45), 0.02, p_rojo2);
Esfera bola5(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.2, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.07, p_amar2);
FuenteLuz f0_2(Punto(camara.getPref()->getX()-0.2, camara.getPref()->getY() - 0.1 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.4), 1.2);
FuenteLuz f1_2(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 0.2 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.2), 2);

//ESCENA 3
Esfera suelo1(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.7, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.6), 0.5, p_verde);
Esfera suelo2(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.7, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.4), 0.5, p_azul);
Esfera suelo3(Punto(camara.getPref()->getX()+0.1, camara.getPref()->getY()-0.7, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.5, p_rojo);
Esfera suelo4(Punto(camara.getPref()->getX()-0.1, camara.getPref()->getY()-0.7, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.5, p_amar);
Esfera e0_3(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.1, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 0.2, p_blanco2);
//Esfera e2(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.1, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.5), 5, reflex);

FuenteLuz f0_3(Punto(camara.getPref()->getX()-0.3, camara.getPref()->getY()+0.8 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.2), 1.5);
FuenteLuz f1_3(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 1 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 2);

//ESCENA 4
Esfera e0_4(Punto(camara.getPref()->getX()-0.5, camara.getPref()->getY()-0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_rojo);
Esfera e1_4(Punto(camara.getPref()->getX()-0.25, camara.getPref()->getY()-0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_rojo2);
Esfera e2_4(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_rojo3);
Esfera e3_4(Punto(camara.getPref()->getX()+0.25, camara.getPref()->getY()-0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_rojo4);
Esfera e4_4(Punto(camara.getPref()->getX()+0.5, camara.getPref()->getY()-0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_rojo5);

Esfera e5_4(Punto(camara.getPref()->getX()-0.5, camara.getPref()->getY()-0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_amar);
Esfera e6_4(Punto(camara.getPref()->getX()-0.25, camara.getPref()->getY()-0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_amar2);
Esfera e7_4(Punto(camara.getPref()->getX(), camara.getPref()->getY()-0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_amar3);
Esfera e8_4(Punto(camara.getPref()->getX()+0.25, camara.getPref()->getY()-0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_amar4);
Esfera e9_4(Punto(camara.getPref()->getX()+0.5, camara.getPref()->getY()-0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_amar5);

Esfera e10_4(Punto(camara.getPref()->getX()-0.5, camara.getPref()->getY()+0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_azul);
Esfera e11_4(Punto(camara.getPref()->getX()-0.25, camara.getPref()->getY()+0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_azul2);
Esfera e12_4(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_azul3);
Esfera e13_4(Punto(camara.getPref()->getX()+0.25, camara.getPref()->getY()+0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_azul4);
Esfera e14_4(Punto(camara.getPref()->getX()+0.5, camara.getPref()->getY()+0.15, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_azul5);

Esfera e15_4(Punto(camara.getPref()->getX()-0.5, camara.getPref()->getY()+0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_verde);
Esfera e16_4(Punto(camara.getPref()->getX()-0.25, camara.getPref()->getY()+0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_verde2);
Esfera e17_4(Punto(camara.getPref()->getX(), camara.getPref()->getY()+0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_verde3);
Esfera e18_4(Punto(camara.getPref()->getX()+0.25, camara.getPref()->getY()+0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_verde4);
Esfera e19_4(Punto(camara.getPref()->getX()+0.5, camara.getPref()->getY()+0.45, camara.getPref()->getZ()+DISTANCIA_PANTALLA+0.25), 0.1, p_verde5);


FuenteLuz f0_4(Punto(camara.getPref()->getX()-0.5, camara.getPref()->getY() +2, camara.getPref()->getZ() -  5), 4);
FuenteLuz f1_4(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 0.5 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.4), 5);

//PROTOTIPOS DE LAS FUNCIONES
void cargar_escena(int escena);
Matriz calcular_locales(Vector normal, Punto posicion);
void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
void iluminacion_indirecta(Punto interseccion, Vector normal, float* fr, Vector omega_o, int esfera, int rebotes, int rebotesInd);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
Vector calcular_refractado(Vector* rayo, Vector* normal, double n1, double n2);
float lanzar_rayo_luz(Rayo* r, int num_luz, float distancia, bool indirecta);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void fRefraccion(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void lanzar_rayos(Punto previo, Punto interseccion, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);


#endif //RAYTRACER_RAYTRACER_H
