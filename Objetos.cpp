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

FuenteLuz FuenteLuz::transformar(Matriz coorLocales){
    Punto nuevoOrigen = coorLocales.transformar(&origen);
    return FuenteLuz(nuevoOrigen, energia);
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

Esfera::Esfera(Punto p, float r, Material material){
	origen = p;
	Punto o(0,0,0);
	vPosicion = Vector::getDireccion(&o,&origen);
	radio = r;
	cc = Vector::pEscalar(&vPosicion,&vPosicion);
	m = material;
}

//Destructor
Esfera::~Esfera(){
}

//Getters
float Esfera::getRadio(){
	return radio;
}

const float* Esfera::getKd(){
	return m.kd;
}

float Esfera::getKs(){
	return m.ks;
}

float Esfera::getKr(){
    return m.kr;
}

float Esfera::getAlpha() {
    return m.alpha;
}

int Esfera::getSuperficie(){
    return m.s;
}

Esfera Esfera::transformar(Matriz coorLocales){
    Punto nuevoOrigen = coorLocales.transformar(&origen);
    return Esfera(nuevoOrigen, radio, m);
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