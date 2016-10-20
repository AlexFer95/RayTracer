#include "Punto.hpp"

//Constructores
Punto::Punto(){
}

Punto::Punto(float x1, float y1, float z1){
	x=x1;
	y=y1;
	z=z1;
}

//Destructor
Punto::~Punto(){
}

//Setters de las cordenadas
void Punto::setX(float x1){
	x=x1;
}

void Punto::setY(float y1){
	y=y1;
}

void Punto::setZ(float z1){
	z=z1;
}
		
//Getters de las coordenadas
float Punto::getX(){
	return x;
}

float Punto::getY(){
	return y;
}

float Punto::getZ(){
	return z;
}

//Devuelve la suma de dos puntos p1+p2
Punto Punto::sumar(Punto* p1, Punto* p2){
	Punto p (p1->getX() + p2->getX(), p1->getY() + p2->getY(), p1->getZ() + p2->getZ());
	return p;
}

//Devuelve la resta de dos puntos p1-p2
Punto Punto::restar(Punto* p1, Punto* p2){
	Punto p (p1->getX() - p2->getX(), p1->getY() - p2->getY(), p1->getZ() - p2->getZ());
	return p;
}

//Delvuelve el nuevo punto tras aplicarle un desplazamiento
Punto Punto::desplazar(Punto* p1, Vector* v1){
	Punto p (p1->getX() + v1->getX(), p1->getY() + v1->getY(), p1->getZ() + v1->getZ());
	return p;
}

