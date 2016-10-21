#include "Vector.hpp"

//Constructores
Vector::Vector(){
}

Vector::Vector(float x1, float y1, float z1){
	x=x1;
	y=y1;
	z=z1;
}

//Destructor
Vector::~Vector(){
}

//Setters de las cordenadas
void Vector::setX(float x1){
	x=x1;
}

void Vector::setY(float y1){
	y=y1;
}

void Vector::setZ(float z1){
	z=z1;
}
		
//Getters de las coordenadas
float Vector::getX(){
	return x;
}

float Vector::getY(){
	return y;
}

float Vector::getZ(){
	return z;
}

//Devuelve el modulo del vector
float Vector::modulo(){
	return sqrt(x*x+y*y+z*z);
}
	
//Normaliza el vector
void Vector::normalizar(){
	float m = modulo();
	x = x/m;
	y = y/m;
	z = z/m;
}

//Devuelve el resultado de un vector multplicado por un escalar,
//obteniendo la misma dirección pero con distinto modulo
Vector Vector::productoEscalar(Vector* v1,float d){
	Vector v(v1->getX()*d, v1->getY()*d, v1->getZ()*d);
	return v;
}

//Devulve el vector entre los dos puntos
Vector Vector::getDireccion(Punto* p1, Punto* p2){
	Vector v(p2->getX() - p1->getX(), p2->getY() - p1->getY(), p2->getZ() - p1->getZ());
	return v;
}

//Devuelve la suma de los vectores v1+v2
Vector Vector::sumar(Vector* v1, Vector* v2){
	Vector v(v1->getX() + v2->getX(), v1->getY() + v2->getY(), v1->getZ() + v2->getZ());
	return v;
}

//Devuelve la resta de los vectores v1-v2
Vector Vector::restar(Vector* v1, Vector* v2){
	Vector v(v1->getX() - v2->getX(), v1->getY() - v2->getY(), v1->getZ() - v2->getZ());
	return v;
}

//Devuelve el producto escalar v1 . v2
float Vector::pEscalar(Vector* v1, Vector* v2){
	
	float m1 = v1->modulo();
	float m2 = v2->modulo();
	
	if(m1==0.0 || m2==0.0){
		return 0;
	}
	else{
		return m1 * m2 * cosenoVector(v1,v2);
	}
}

//Devuelve el producto vectorial v1 x v2
Vector Vector::pVectorial(Vector* v1, Vector* v2){
	Vector v(v1->getY() * v2->getZ() - v1->getZ() * v2->getY(),
		   -(v1->getX() * v2->getZ() - v1->getZ() * v2->getX()),
			(v1->getX() * v2->getY() - v1->getY() * v2->getX()));
	return v;
}

float Vector::cosenoVector(Vector* v1, Vector* v2){
    return (v1->getX() * v2->getX() +  v1->getY() * v2->getY() + v1->getZ() * v2->getZ())/
           (sqrt( pow(v1->getX(), 2) + pow(v1->getY(), 2) + pow(v1->getZ(), 2))*
            sqrt( pow(v2->getX(), 2) + pow(v2->getY(), 2) + pow(v2->getZ(), 2)));
}