#ifndef MATRIZ_HPP
#define MATRIZ_HPP

#include "Punto.hpp"
#include "Vector.hpp"

class Punto;
class Vector;

/**
 **	Matriz de cambio de base entre sistemas de ejes coordenados y de transformaciones
 **	a -> right
 **	b -> top
 **	c -> front
 **/
class Matriz{
	private:
		//Vectores y punto que forman la matriz
		Vector a;
		Vector b; 
		Vector c;
		Punto p;
		
	public:
		//Constructores
		Matriz();
		Matriz(Vector v1, Vector v2, Vector v3, Punto p1);
		//Destructor
		~Matriz();
		
		//Setters de los vectores y el punto
		void setA(Vector a1);
		void setB(Vector b1);
		void setC(Vector c1);
		void setP(Punto p);
		
		
		//Getters de los vectores y el punto
		Vector getA();
		Vector getB();
		Vector getC();
		Punto getP();
		Vector* getAref();
		Vector* getBref();
		Vector* getCref();
		Punto* getPref();
		
		//Transformaciones con la matriz
		Vector transformar(Vector* v);
		Punto transformar(Punto* p);
		
		//Devuelve la suma de dos matrices m1+m2
		static Matriz sumar(Matriz* m1, Matriz* m2);
		//Devuelve la resta de dos matrices m1-m2
		static Matriz restar(Matriz* m1, Matriz* m2);
		//Devuelve el producto de dos matrices m1*m2
		static Matriz multiplicar(Matriz* m1, Matriz* m2);
};

#endif