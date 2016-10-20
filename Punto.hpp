#ifndef PUNTO_HPP
#define PUNTO_HPP

#include "Vector.hpp"

class Vector;

class Punto{
	private:
		//Coordenadas del punto
		float x; // Right
		float y; // Top
		float z; // Front
		
	public:
		//Constructores
		Punto();
		Punto(float x1, float x2, float x3);
		//Destructor
		~Punto();
		
		//Setters de las cordenadas
		void setX(float x1);
		void setY(float y1);
		void setZ(float z1);
		
		//Getters de las coordenadas
		float getX();
		float getY();
		float getZ();
		
		//Devuelve la suma de dos puntos p1+p2
		static Punto sumar(Punto* p1, Punto* p2);
		//Devuelve la resta de dos puntos p1-p2
		static Punto restar(Punto* p1, Punto* p2);
		//Delvuelve el nuevo punto tras aplicarle un desplazamiento
		static Punto desplazar(Punto* p1, Vector* v1);
};

#endif