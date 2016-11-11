#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"

enum{
	Phong = 0,
	Reflexion = 1,
	Refraccion = 2,
};

class Objeto{
	protected:
		Punto origen;
		Vector vPosicion;

	public:
		//Constructores
		Objeto();
		Objeto(float x, float y, float z);
		//Destructor
		~Objeto();

		//Getters
		Punto getOrigen();
		Vector getVPosicion();
};

class FuenteLuz : public Objeto{
	private:
		float energia;
		
	public:
		//Constructores
		FuenteLuz();
		FuenteLuz(Punto p, float e);
		//Destructor
		~FuenteLuz();
		
		//Getters
		float getEnergia();
};

class Rayo : public Objeto{
	private:
		Vector direccion;
		//Datos invariantes a calcular
		float oo;
		float od;
		float dd;
		
	public:
		//Constructores
		Rayo();
		Rayo(Punto p, Vector d);
		//Destructor
		~Rayo();
		
		//Getters
		Vector getDireccion();
		float getOO();
		float getOD();
		float getDD();
};

class Esfera : public Objeto{
	private:
		float radio;
		
		//Coeficientes [0,1] por el que multiplicar la intensidad
		float kd[3];
        float ks;


		int superficie;

		//Datos invariates a calcular
		float cc;
		float oc;
		float dc;
		
	public:
		//Constructores
		Esfera();
		Esfera(Punto p, float r, float cr=1.0, float cg=1.0, float cb=1.0, float cs=1.0, int superfice = Phong);
		//Destructor
		~Esfera();
		
		//Getters
		float getRadio();
		float* getKd();
        float getKs();
		int getSuperficie();
		
		//Calcula la distancia de interseccion del rayo con la esfera
		// Guarda en sol[0] la solucion con la raiz positiva o NaN si no existe
		// Guarda en sol[1] la solucion con la raiz negatica o NaN si no existe
		void intersectar(Rayo* ray, float sol[2]);
};

#endif