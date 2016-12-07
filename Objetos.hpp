#ifndef OBJETOS_HPP
#define OBJETOS_HPP

#include <iostream>

#include "Vector.hpp"
#include "Punto.hpp"
#include "Matriz.hpp"

//COLORES
const float amarillo[3] = { 1.0 , 1.0 , 0.1 };
const float azul[3] = { 0.1 , 1.0 , 1.0 };
const float azul2[3] = { 0.1 , 0.1 , 1.0 };
const float morado[3] = { 1.0 , 0.1 , 1.0 };
const float negro[3] = { 0.0 , 0.0 , 0.0 };
const float rojo[3] = { 1.0 , 0.1 , 0.1 };

enum Superficie{
	Phong = 0,
	Reflexion = 1,
	Refraccion = 2,
};

struct Material{
	const float* kd;
	float ks;
	float kr;
    float absorcion;
	Superficie s;
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

        FuenteLuz transformar(Matriz coorLocales);
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
		
		//Propiedades materiales de la esfera
		struct Material m;

		//Datos invariates a calcular
		float cc;
		float oc;
		float dc;
		
	public:
		//Constructores
		Esfera();
		Esfera(Punto p, float r, Material material);
		//Destructor
		~Esfera();
		
		//Getters
		float getRadio();
		const float* getKd();
        float getKs();
        float getKr();
		int getSuperficie();

        Esfera transformar(Matriz coorLocales);
		
		//Calcula la distancia de interseccion del rayo con la esfera
		// Guarda en sol[0] la solucion con la raiz positiva o NaN si no existe
		// Guarda en sol[1] la solucion con la raiz negatica o NaN si no existe
		void intersectar(Rayo* ray, float sol[2]);
};

#endif