#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Punto.hpp"
#include <math.h>

class Punto;

class Vector{
	private:
		//Coordenadas del vector
		float x; // Right
		float y; // Top
		float z; // Front
		
	public:
		//Constructores
		Vector();
		Vector(float x1, float y1, float z1);
		//Destructor
		~Vector();
		
		//Setters de las cordenadas
		void setX(float x1);
		void setY(float y1);
		void setZ(float z1);
		
		//Getters de las coordenadas
		float getX();
		float getY();
		float getZ();
		
		//Devuelve el modulo del vector
		float modulo();
		//Normaliza el vector
		void normalizar();
		
		//Devuelve el resultado de un vector multplicado por un escalar,
		//obteniendo la misma dirección pero con distinto modulo
		static Vector productoEscalar(Vector* v1,float d);
		
		//Devulve el vector entre los dos puntos
		static Vector getDireccion(Punto* p1, Punto* p2);
		
		//Devuelve la suma de los vectores v1+v2
		static Vector sumar(Vector* v1, Vector* v2);
		//Devuelve la resta de los vectores v1-v2
		static Vector restar(Vector* v1, Vector* v2);
		
		//Devuelve el producto escalar v1 . v2
		static float pEscalar(Vector* v1, Vector* v2);
		//Devuelve el producto vectorial v1 x v2
		static Vector pVectorial(Vector* v1, Vector* v2);
};

#endif