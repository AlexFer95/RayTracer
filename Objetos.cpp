#include "Objetos.hpp"

/*****************************************************************************/
/***********************************OBJETO************************************/
/*****************************************************************************/

//Constructores
Objeto::Objeto(){
}
		
Objeto::Objeto(float x, float y, float z){
	origen = Punto(x,y,z);
	Punto p(0,0,0);
	vPosicion = Vector::getDireccion(&p,&origen);
}

//Destructor
Objeto::~Objeto(){
}

//Getters
Punto Objeto::getOrigen(){
	return origen;
}

Vector Objeto::getVPosicion(){
	return vPosicion;
}

/*****************************************************************************/
/**********************************FUENTELUZ**********************************/
/*****************************************************************************/

//Constructores
FuenteLuz::FuenteLuz(){
}

FuenteLuz::FuenteLuz(Punto p, float e){
	origen = p;
	Punto o(0,0,0);
	vPosicion = Vector::getDireccion(&o,&origen);
	energia = e;
}

//Destructor
FuenteLuz::~FuenteLuz(){
}
		
//Getters
float FuenteLuz::getEnergia(){
	return energia;
}

/*****************************************************************************/
/************************************RAYO*************************************/
/*****************************************************************************/

//Constructores
Rayo::Rayo(){
}

Rayo::Rayo(Punto p, Vector d){
	origen = p;
	Punto o(0,0,0);
	vPosicion = Vector::getDireccion(&o,&origen);
	direccion = d;
	oo = Vector::pEscalar(&vPosicion,&vPosicion);
	od = Vector::pEscalar(&vPosicion,&direccion);
	dd = Vector::pEscalar(&direccion,&direccion);
}

//Destructores
Rayo::~Rayo(){
}
		
//Getters
Vector Rayo::getDireccion(){
	return direccion;
}

float Rayo::getOO(){
	return oo;
}

float Rayo::getOD(){
	return od;
}

float Rayo::getDD(){
	return dd;
}

/*****************************************************************************/
/***********************************ESFERA************************************/
/*****************************************************************************/

//Constructores
Esfera::Esfera(){
}

Esfera::Esfera(Punto p, float r, float cr, float cg, float cb){
	origen = p;
	Punto o(0,0,0);
	vPosicion = Vector::getDireccion(&o,&origen);
	radio = r;
	cc = Vector::pEscalar(&vPosicion,&vPosicion);
	kd[0] = cr;
    kd[1] = cg;
    kd[2] = cb;
	ks = cg;
}

//Destructor
Esfera::~Esfera(){
}
		
//Getters
float Esfera::getRadio(){
	return radio;
}

float* Esfera::getKd(){
	return kd;
}

float Esfera::getKs(){
	return ks;
}
		
//Calcula la distancia de interseccion del rayo con la esfera
// Guarda en sol[0] la solucion con la raiz positiva o NaN si no existe
// Guarda en sol[1] la solucion con la raiz negatica o NaN si no existe
void Esfera::intersectar(Rayo* ray, float sol[2]){
	Vector ori = ray->getVPosicion();
	oc = Vector::pEscalar(&ori,&vPosicion);
	Vector dir = ray->getDireccion();
	dc = Vector::pEscalar(&dir,&vPosicion);
	
	float a = ray->getDD();
	float b = 2*ray->getOD() - 2*dc;
	float c = ray->getOO() + cc - 2*oc - radio*radio;
	
	float raiz = sqrt(b*b - 4*a*c);
	
	sol[0] = (-b + raiz) / 2*a;
	sol[1] = (-b - raiz) / 2*a;
}