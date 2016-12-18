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
const int ANCHO_IMAGEN = 250;
const int ALTO_IMAGEN = ANCHO_IMAGEN;
const float COLOR_IMAGEN = 255;
const float DISTANCIA_PANTALLA = 0.5;
const float ANCHO_PANTALLA = 1;
const float ALTO_PANTALLA = 1;
const float TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;
const int MAX_RAYOS = 16;
const int MAX_REBOTES_IND = 1;

//VARIABLES DEL TRAZADOR
float*** buffer;
std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0.0,1.0);
FuenteLuz *lista_luces[20];
Esfera *lista_esferas[20];
int num_luces;
int num_esferas;

Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0)); //camara(matriz)
/*//Definir fuentes de luz
FuenteLuz f0(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
FuenteLuz f1(Punto(camara.getPref()->getX()-DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
int num_luces = 2;
FuenteLuz *lista_luces[] = {&f0,&f1};*/

//Definir objetos de la escena
Material p_azul = { azul , 0.0 , 0.0 , 0.1 , Phong};
Material p_amar = { amarillo , 0.0 , 0.0 , 0.1 , Phong};
Material p_mora = { morado , 0.0 , 0.0 , 0.1 , Phong};
Material p_rojo = { rojo , 0.1 , 0.0 , 0.1 , Phong};
Material p_azul2 = { azul2 , 0.0 , 0.0 , 0.1 , Phong};
Material p_verde = { verde , 0.1 , 0.0 , 0.1 , Phong};
Material p_gris = { gris , 0.1 , 0.0 , 0.1 , Phong};
Material reflex = { negro , 0.0 , 0.0 , 0.1 , Reflexion};
Material refrac = { negro , 0.0 , 1.6 , 0.1 , Refraccion};

//ESCENA 1: CORNELL BOX (1x1)
Esfera cv_left(Punto(camara.getPref()->getX() - 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + 0.5), 1000, p_rojo);
Esfera cv_right(Punto(camara.getPref()->getX() + 1000.5, camara.getPref()->getY(), camara.getPref()->getZ() + 0.5), 1000, p_verde);
Esfera cv_floor(Punto(camara.getPref()->getX() , camara.getPref()->getY() - 1000.5, camara.getPref()->getZ() + 0.5), 1000, p_gris);
Esfera cv_roof(Punto(camara.getPref()->getX() , camara.getPref()->getY() + 1000.5, camara.getPref()->getZ() + 0.5), 1000, p_gris);
Esfera cv_back(Punto(camara.getPref()->getX() , camara.getPref()->getY(), camara.getPref()->getZ() + 1001), 1000, p_gris);
FuenteLuz f0(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ()), 2000);



/*Esfera suelo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_azul);
Esfera techo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*-1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_mora);
Esfera fondo(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*1020), DISTANCIA_PANTALLA*1000, p_amar);
Esfera frente(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*-1010), DISTANCIA_PANTALLA*1000, p_amar);
Esfera izquierda(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*-1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_mora);
Esfera derecha(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_mora);
Esfera e0(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA/2, p_rojo);
Esfera e1(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*4, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, p_azul);
Esfera e2(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*-4, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, p_amar);
int num_esferas = 7;
Esfera *lista_esferas[] = {&suelo,&techo,&fondo,&frente,&izquierda,&derecha,&e0/*,&e1,&e2};*/

void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum, float distancia);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);

Matriz calcular_locales(Vector normal, Punto posicion);
void cambiar_coord_escena(Esfera esferasLocales[], FuenteLuz lucesLocales[], Matriz T);
void reestablecer_globales();
#endif //RAYTRACER_RAYTRACER_H
