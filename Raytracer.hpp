//
// Created by adrian on 11/11/2016.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include "Punto.hpp"
#include "Vector.hpp"
#include "Matriz.hpp"
#include "Objetos.hpp"

#define PI 3.14159
#define ALPHA 70
#define MAX_REBOTES 4
#define EPSILON 0.001
#define NUM_ESCENAS 1

//CONSTANTES DE LA IMAGEN Y LA PANTALLA
const int ANCHO_IMAGEN = 200;
const int ALTO_IMAGEN = ANCHO_IMAGEN;
const float COLOR_IMAGEN = 255;
const float DISTANCIA_PANTALLA = 0.5;
const float ANCHO_PANTALLA = 1;
const float ALTO_PANTALLA = 1;
const float TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;
const int MAX_RAYOS = 32;
const int MAX_REBOTES_IND = 1;

//COLORES
const float amarillo[3] = { 0.4 , 0.4 , 0.2 };
const float azul[3] = { 0.2 , 0.4 , 0.4 };
const float azul2[3] = { 0.1 , 0.1 , 0.8 };
const float morado[3] = { 0.4 , 0.2 , 0.4 };
const float negro[3] = { 0.0 , 0.0 , 0.0 };
const float rojo[3] = { 0.8 , 0.1 , 0.1 };
const float verde[3] = { 0.1 , 0.8 , 0.1 };
const float gris[3] = { 0.3 , 0.3 , 0.3};
const float blanco[3] = {1.0 , 1.0 , 1.0};

//VARIABLES DEL TRAZADOR
float*** buffer;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0,1.0);

FuenteLuz *lista_luces[20];
Esfera *lista_esferas[20];
int num_luces;
int num_esferas;

Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0));

//Definir materiales para las escenas
Material p_azul = { azul , 0.0 , 0.0 , Phong};
Material p_amar = { amarillo , 0.0 , 0.0 , Phong};
Material p_mora = { morado , 0.0 , 0.0 , Phong};
Material p_rojo = { rojo , 0.0 , 0.0 , Phong};
Material p_azul2 = { azul2 , 0.0 , 0.0 , Phong};
Material p_verde = { verde , 0.0 , 0.0 , Phong};
Material p_gris = { gris , 0.0 , 0.0 , Phong};
Material reflex = { blanco , 0.0 , 0.0 , Reflexion};
Material refrac = { blanco , 0.0, 1.6 , Refraccion};

//ESCENA 1: CORNELL BOX (1x1)
Esfera cv_left(Punto(camara.getPref()->getX() - 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_rojo);
Esfera cv_right(Punto(camara.getPref()->getX() + 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_verde);
Esfera cv_floor(Punto(camara.getPref()->getX() , camara.getPref()->getY() - 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_gris);
Esfera cv_roof(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 1000.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.5), 1000, p_gris);
Esfera cv_back(Punto(camara.getPref()->getX() , camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA + 1001), 1000, p_gris);
Esfera cv_spe(Punto(camara.getPref()->getX() - 0.15, camara.getPref()->getY() - 0.25, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.7), 0.25, p_azul);
Esfera cv_ref(Punto(camara.getPref()->getX() + 0.3, camara.getPref()->getY() - 0.35, camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.4), 0.15, refrac);
FuenteLuz f0(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 0.1 , camara.getPref()->getZ() + DISTANCIA_PANTALLA + 0.2), 1000);

//PROTOTIPOS DE LAS FUNCIONES
void cargar_escena(int escena);
Matriz calcular_locales(Vector normal, Punto posicion);
void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
void iluminacion_indirecta(Punto interseccion, Vector normal, float* fr, Vector omega_o, int esfera, int rebotes);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
Vector calcular_refractado(Vector* rayo, Vector* normal, double n1, double n2);
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum, float distancia);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void fRefraccion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);


#endif //RAYTRACER_RAYTRACER_H
