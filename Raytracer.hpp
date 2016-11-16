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
const float ANCHO_IMAGEN = 700;
const float ALTO_IMAGEN = ANCHO_IMAGEN;
const float COLOR_IMAGEN = 255;
const float DISTANCIA_PANTALLA = 0.1;
const float ANCHO_PANTALLA = 2*tan(30*PI/180)*DISTANCIA_PANTALLA;
const float ALTO_PANTALLA = ANCHO_PANTALLA;
const float TAM_PIXEL = ANCHO_PANTALLA / ANCHO_IMAGEN;

//VARIABLES DEL TRAZADOR
Punto origen(0, 0, 0); //Origen del sistema
Matriz camara(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Punto(ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2, 0)); //camara(matriz)
//Definir fuentes de luz
FuenteLuz f0(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
FuenteLuz f1(Punto(camara.getPref()->getX()-DISTANCIA_PANTALLA*8, camara.getPref()->getY()+DISTANCIA_PANTALLA*4, DISTANCIA_PANTALLA*8), 2000);
int num_luces = 2;
FuenteLuz *lista_luces[] = {&f0,&f1};
//Definir objetos de la escena
Esfera suelo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, 0.1, 1, 1, 0.05);
Esfera techo(Punto(camara.getPref()->getX(), camara.getPref()->getY() + DISTANCIA_PANTALLA*-1005, camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, 0.1, 1, 1, 0.05);
Esfera fondo(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*1020), DISTANCIA_PANTALLA*1000, 1, 1, 0.1, 0.05);
Esfera frente(Punto(camara.getPref()->getX(), camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*-1010), DISTANCIA_PANTALLA*1000, 1, 1, 0.1, 0.05);
Esfera izquierda(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*-1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, 1, 0.1, 1, 0.05);
Esfera derecha(Punto(camara.getPref()->getX() + DISTANCIA_PANTALLA*1010, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*20), DISTANCIA_PANTALLA*1000, 1, 0.1, 1, 0.05);
Esfera e0(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*4, camara.getPref()->getY()-DISTANCIA_PANTALLA*1.5, camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA/2, 1, 0.1, 0.1, 0.1);
Esfera e1(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA, camara.getPref()->getY()-DISTANCIA_PANTALLA, camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA, 0.1, 1, 0.1, 0.5, Refraccion, 1.2);
Esfera e2(Punto(camara.getPref()->getX()+DISTANCIA_PANTALLA*-4, camara.getPref()->getY(), camara.getPref()->getZ() + DISTANCIA_PANTALLA*12), DISTANCIA_PANTALLA*2, 0.1, 0.1, 1, 0.9);
int num_esferas = 9;
Esfera *lista_esferas[] = {&suelo,&techo,&fondo,&frente,&izquierda,&derecha,&e0,&e1,&e2};

void brdf(Vector* omega_i, Vector* omega_r, int ultima_esfera, float* fr);
Vector calcular_reflejado(Vector* rayo, Vector* normal);
float lanzar_rayo_luz(Rayo* r, int num_luz, float dist_acum, float distancia);
void colisionRayoObjetos(Rayo* r, int* i, float* j);
void fPhong(Punto previo, Punto interseccion, float dist_acum, int ultima, float* intensidad);
void fReflexion(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad);
void lanzar_secundarios(Punto previo, Punto interseccion, float dist_acum, int ultima, int rebotes, float* intensidad);

#endif //RAYTRACER_RAYTRACER_H
