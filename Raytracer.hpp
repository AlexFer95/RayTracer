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

//CONSTANTES DE LA IMAGEN Y LA PANTALLA
const float ANCHO_IMAGEN = 600;
const float ALTO_IMAGEN = ANCHO_IMAGEN;
const float COLOR_IMAGEN = 255;
const float DISTANCIA_PANTALLA = 0.1;
const float ANCHO_PANTALLA = 2*tan(30*PI/180)*DISTANCIA_PANTALLA;
const float ALTO_PANTALLA = ANCHO_PANTALLA;
const float TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;
const int MAX_RAYOS = 100;
const int MAX_REBOTES_IND = 1;
//VARIABLES DEL TRAZADOR
Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0)); //camara(matriz)
//Definir fuentes de luz
FuenteLuz f0(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
FuenteLuz f1(Punto(camara.getPref()->getX()-DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
int num_luces = 2;
FuenteLuz *lista_luces[] = {&f0,&f1};

//Definir objetos de la escena
Material p_azul = { azul , 0.0 , 0.0 , 0.1 , Phong};
Material p_amar = { amarillo , 0.0 , 0.0 , 0.1 , Phong};
Material p_mora = { morado , 0.0 , 0.0 , 0.1 , Phong};
Material p_rojo = { rojo , 0.1 , 0.0 , 0.1 , Phong};
Material p_azul2 = { azul2 , 0.0 , 0.0 , 0.1 , Phong};
Material reflex = { negro , 0.0 , 0.0 , 0.1 , Reflexion};
Material refrac = { negro , 0.0 , 1.6 , 0.1 , Refraccion};

Esfera suelo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_azul);
Esfera techo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*-1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_azul);
Esfera fondo(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*1020), DISTANCIA_PANTALLA*1000, p_amar);
Esfera frente(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*-1010), DISTANCIA_PANTALLA*1000, p_amar);
Esfera izquierda(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*-1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_mora);
Esfera derecha(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, p_mora);
Esfera e0(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA/2, p_rojo);
Esfera e1(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*4, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, p_azul);
Esfera e2(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*-4, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, p_amar);
int num_esferas = 9;
Esfera *lista_esferas[] = {&suelo,&techo,&fondo,&frente,&izquierda,&derecha,&e0,&e1,&e2};

void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum, float distancia);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, int rebotesIndirectos, float* intensidad);

Matriz calcular_locales(Vector normal, Punto posicion);
Esfera cambiar_escena_locales(Esfera esferasLocales[], FuenteLuz lucesLocales[], Matriz T);
#endif //RAYTRACER_RAYTRACER_H
