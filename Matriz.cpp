#include "Matriz.hpp"

//Constructores
Matriz::Matriz(){
}

Matriz::Matriz (Vector v1, Vector v2, Vector v3, Punto p1){
	a = v1; //right
	b = v2; //top
	c = v3; //front
	p = p1;
}

//Destructor
Matriz::~Matriz(){
}

//Setters de los vectores y el punto
void Matriz::setA(Vector a1){
	a=a1;
}

void Matriz::setB(Vector b1){
	b=b1;
}

void Matriz::setC(Vector c1){
	c=c1;
}

void Matriz::setP(Punto p1){
	p=p1;
}
		
//Getters de los vectores y el punto
Vector Matriz::getA(){
	return a;
}

Vector Matriz::getB(){
	return b;
}

Vector Matriz::getC(){
	return c;
}

Punto Matriz::getP(){
	return p;
}

Vector* Matriz::getAref(){
	return &a;
}

Vector* Matriz::getBref(){
	return &b;
}

Vector* Matriz::getCref(){
	return &c;
}

Punto* Matriz::getPref(){
	return &p;
}

//Transformaciones con la matriz
Vector Matriz::transformar (Vector* v){
	Vector v1(a.getX()*v->getX() + b.getX()*v->getY() + c.getX()*v->getZ(), 
			  a.getY()*v->getX() + b.getY()*v->getY() + c.getY()*v->getZ(),
			  a.getZ()*v->getX() + b.getZ()*v->getY() + c.getZ()*v->getZ());
	return v1;
}
		
Punto Matriz::transformar (Punto* p){			
	Punto p1(a.getX()*p->getX() + b.getX()*p->getY() + c.getX()*p->getZ() + p->getX(), 
			 a.getY()*p->getX() + b.getY()*p->getY() + c.getY()*p->getZ() + p->getY(),
			 a.getZ()*p->getX() + b.getZ()*p->getY() + c.getZ()*p->getZ() + p->getZ());
	return p1;
}

//Devuelve la suma de dos matrices m1+m2
Matriz Matriz::sumar(Matriz* m1, Matriz* m2){
	Matriz suma(Vector::sumar(m1->getAref(), m2->getAref()),
				Vector::sumar(m1->getBref(), m2->getBref()),
				Vector::sumar(m1->getCref(), m2->getCref()),
				Punto::sumar(m1->getPref(), m2->getPref()));
	return suma;
}

//Devuelve la resta de dos matrices m1-m2
Matriz Matriz::restar(Matriz* m1, Matriz* m2){
	Matriz resta(Vector::restar(m1->getAref(), m2->getAref()),
				 Vector::restar(m1->getBref(), m2->getBref()),
				 Vector::restar(m1->getCref(), m2->getCref()),
				 Punto::restar(m1->getPref(), m2->getPref()));
	return resta;
}
//Devuelve el producto de dos matrices m1*m2
Matriz Matriz::multiplicar(Matriz* m1, Matriz* m2){
	Vector v1(m1->getA().getX()*m2->getA().getX() + m1->getA().getY()*m2->getB().getX() + m1->getA().getZ()*m2->getC().getX(), 
			  m1->getA().getX()*m2->getA().getY() + m1->getA().getY()*m2->getB().getY() + m1->getA().getZ()*m2->getC().getY(),
			  m1->getA().getX()*m2->getA().getZ() + m1->getA().getY()*m2->getB().getZ() + m1->getA().getZ()*m2->getC().getZ());
	Vector v2(m1->getB().getX()*m2->getA().getX() + m1->getB().getY()*m2->getB().getX() + m1->getB().getZ()*m2->getC().getX(), 
			  m1->getB().getX()*m2->getA().getY() + m1->getB().getY()*m2->getB().getY() + m1->getB().getZ()*m2->getC().getY(),
			  m1->getB().getX()*m2->getA().getZ() + m1->getB().getY()*m2->getB().getZ() + m1->getB().getZ()*m2->getC().getZ());
	Vector v3(m1->getC().getX()*m2->getA().getX() + m1->getC().getY()*m2->getB().getX() + m1->getC().getZ()*m2->getC().getX(), 
			  m1->getC().getX()*m2->getA().getY() + m1->getC().getY()*m2->getB().getY() + m1->getC().getZ()*m2->getC().getY(),
			  m1->getC().getX()*m2->getA().getZ() + m1->getC().getY()*m2->getB().getZ() + m1->getC().getZ()*m2->getC().getZ());
	Punto p1(m1->getP().getX()*m2->getA().getX() + m1->getP().getY()*m2->getB().getX() + m1->getP().getZ()*m2->getC().getX() + m2->getP().getX(), 
			 m1->getP().getX()*m2->getA().getY() + m1->getP().getY()*m2->getB().getY() + m1->getP().getZ()*m2->getC().getY() + m2->getP().getY(),
			 m1->getP().getX()*m2->getA().getZ() + m1->getP().getY()*m2->getB().getZ() + m1->getP().getZ()*m2->getC().getZ() + m2->getP().getZ());
	Matriz m(v1, v2, v3, p1);
	return m;
}