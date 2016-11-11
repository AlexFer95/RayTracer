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
const float ANCHO_PANTALLA = 2*tan(30*PI/180)*0.1;
const float ALTO_PANTALLA = ANCHO_PANTALLA;
const float TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;

//VARIABLES DEL TRAZADOR
Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0)); //camara(matriz)
//Definir fuentes de luz
FuenteLuz f0(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*6, camara.getPref()->getY()+DISTANCIA_PANTALLA*3, DISTANCIA_PANTALLA*10), 1000);
FuenteLuz f1(Punto(camara.getPref()->getX()-DISTANCIA_PANTALLA*6, camara.getPref()->getY()+DISTANCIA_PANTALLA*3, DISTANCIA_PANTALLA*10), 1000);
int num_luces = 2;
FuenteLuz *lista_luces[] = {&f0,&f1};
//Definir objetos de la escena
Esfera e0(Punto(camara.getPref()->getX(), camara.getPref()->getY()-DISTANCIA_PANTALLA*402, DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*400, 0.3, 1, 1, 0);
Esfera e1(Punto(camara.getPref()->getX(), camara.getPref()->getY(), DISTANCIA_PANTALLA*414), DISTANCIA_PANTALLA*400, 1, 1, 0.3, 0);
Esfera e2(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*4, camara.getPref()->getY()-DISTANCIA_PANTALLA*1.5, DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA/2, 1, 0.1, 0.1, 0.1);
Esfera e3(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA, camara.getPref()->getY()-DISTANCIA_PANTALLA, DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA, 0.1, 1, 0.1, 0.5, Reflexion);
Esfera e4(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*-4, camara.getPref()->getY(), DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, 0.1, 0.1, 1, 0.9);
int num_esferas = 5;
Esfera *lista_esferas[] = {&e0,&e1,&e2,&e3,&e4};

void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad);

#endif //RAYTRACER_RAYTRACER_H
