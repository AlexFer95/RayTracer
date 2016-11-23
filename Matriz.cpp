#include "Matriz.hpp"

//Constructores
Matriz::Matriz(){
}

Matriz::Matriz (Vector v1, Vector v2, Vector v3, Punto p1){
	a = v1; //right
	b = v2; //top
	c = v3; //front
	p = p1;
	h[0] = 0;
	h[1] = 0;
	h[2] = 0;
	h[3] = 1;

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

void Matriz::setH(float h0, float h1, float h2, float h3) {
	h[0] = h0;
	h[1] = h1;
	h[2] = h2;
	h[3] = h3;
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

float* Matriz::getH() {
	return h;
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
Matriz Matriz::invertir(){

	double inv[16];
	inv[0] = b.getY()  * c.getZ() * h[3] -
			 b.getY()  * h[2] * c.getZ() -
			 c.getY()  * b.getZ()  * h[3] +
			 c.getY()  * h[1]  * c.getZ() +
			 c.getY() * b.getZ()  * h[2] -
			 c.getY() * h[1]  * c.getZ();

	inv[4] = -b.getX()  * c.getZ() * h[3] +
			 b.getX()  * h[2] * c.getZ() +
			 c.getX()  * b.getZ()  * h[3] -
			 c.getX()  * h[1]  * c.getZ() -
			 c.getX() * b.getZ()  * h[2] +
			 c.getX() * h[1]  * c.getZ();

	inv[8] = b.getX()  * c.getY() * h[3] -
			 b.getX()  * h[2] * c.getY() -
			 c.getX()  * b.getY() * h[3] +
			 c.getX()  * h[1] * c.getY() +
			 c.getX() * b.getY() * h[2] -
			 c.getX() * h[1] * c.getY();

	inv[12] = -b.getX()  * c.getY() * c.getZ() +
			  b.getX()  * c.getZ() * c.getY() +
			  c.getX()  * b.getY() * c.getZ() -
			  c.getX()  * b.getZ() * c.getY() -
			  c.getX() * b.getY() * c.getZ() +
			  c.getX() * b.getZ() * c.getY();

	inv[1] = -a.getY()  * c.getZ() * h[3] +
			 a.getY()  * h[2] * c.getZ() +
			 c.getY()  * a.getZ() * h[3] -
			 c.getY()  * h[0] * c.getZ() -
			 c.getY() * a.getZ() * h[2] +
			 c.getY() * h[0] * c.getZ();

	inv[5] = a.getX()  * c.getZ() * h[3] -
			 a.getX()  * h[2] * c.getZ() -
			 c.getX()  * a.getZ() * h[3] +
			 c.getX()  * h[0] * c.getZ() +
			 c.getX() * a.getZ() * h[2] -
			 c.getX() * h[0] * c.getZ();

	inv[9] = -a.getX()  * c.getY() * h[3] +
			 a.getX()  * h[2] * c.getY() +
			 c.getX()  * a.getY() * h[3] -
			 c.getX()  * h[0] * c.getY() -
			 c.getX() * a.getY() * h[2] +
			 c.getX() * h[0] * c.getY();

	inv[13] = a.getX()  * c.getY() * c.getZ() -
			  a.getX()  * c.getZ() * c.getY() -
			  c.getX()  * a.getY() * c.getZ() +
			  c.getX()  * a.getZ() * c.getY() +
			  c.getX() * a.getY() * c.getZ() -
			  c.getX() * a.getZ() * c.getY();

	inv[2] = a.getY()  * b.getZ() * h[3] -
			 a.getY()  * h[1] * c.getZ() -
			 b.getY()  * a.getZ() * h[3] +
			 b.getY()  * h[0] * c.getZ() +
			 c.getY() * a.getZ() * h[1] -
			 c.getY() * h[0] * b.getZ();

	inv[6] = -a.getX()  * b.getZ() * h[3] +
			 a.getX()  * h[1] * c.getZ() +
			 b.getX()  * a.getZ() * h[3] -
			 b.getX()  * h[0] * c.getZ() -
			 c.getX() * a.getZ() * h[1] +
			 c.getX() * h[0] * b.getZ();

	inv[10] = a.getX()  * b.getY() * h[3] -
			  a.getX()  * h[1] * c.getY() -
			  b.getX()  * a.getY() * h[3] +
			  b.getX()  * h[0] * c.getY() +
			  c.getX() * a.getY() * h[1] -
			  c.getX() * h[0] * b.getY();

	inv[14] = -a.getX()  * b.getY() * c.getZ() +
			  a.getX()  * b.getZ() * c.getY() +
			  b.getX()  * a.getY() * c.getZ() -
			  b.getX()  * a.getZ() * c.getY() -
			  c.getX() * a.getY() * b.getZ() +
			  c.getX() * a.getZ() * b.getY();

	inv[3] = -a.getY() * b.getZ() * h[2] +
			 a.getY() * h[1] * c.getZ() +
			 b.getY() * a.getZ() * h[2] -
			 b.getY() * h[0] * c.getZ() -
			 c.getY() * a.getZ() * h[1] +
			 c.getY() * h[0] * b.getZ();

	inv[7] = a.getX() * b.getZ() * h[2] -
			 a.getX() * h[1] * c.getZ() -
			 b.getX() * a.getZ() * h[2] +
			 b.getX() * h[0] * c.getZ() +
			 c.getX() * a.getZ() * h[1] -
			 c.getX() * h[0] * b.getZ();

	inv[11] = -a.getX() * b.getY() * h[2] +
			  a.getX() * h[1] * c.getY() +
			  b.getX() * a.getY() * h[2] -
			  b.getX() * h[0] * c.getY() -
			  c.getX() * a.getY() * h[1] +
			  c.getX() * h[0] * b.getY();

	inv[15] = a.getX() * b.getY() * c.getZ() -
			  a.getX() * b.getZ() * c.getY() -
			  b.getX() * a.getY() * c.getZ() +
			  b.getX() * a.getZ() * c.getY() +
			  c.getX() * a.getY() * b.getZ() -
			  c.getX() * a.getZ() * b.getY();


	float det = a.getX() * inv[0] + a.getY() * inv[4] + a.getZ() * inv[8] + 0 * inv[12];
	det = 1.0 / det;

	for (int i = 0; i < 16; i++) {
		inv[i] = inv[i] * det;
	}

	Vector v1(inv[0], inv[1], inv[2]);
	Vector v2(inv[4], inv[5], inv[6]);
	Vector v3(inv[8], inv[9], inv[10]);
	Punto p1(inv[12], inv[13], inv[14]);

	Matriz m(v1,v2,v3,p1);
	m.setH(inv[3],inv[7],inv[11],inv[15]);
	return m;
}

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