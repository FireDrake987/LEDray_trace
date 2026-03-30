#include "Plane.h"
#include "Point3D.h"
#include "Vector.h"
#include "Material.h"

Plane::Plane(Material material, Point3D p1, Point3D p2, Point3D p3) {
	this->mat = material;
	Vector v1 = Vector(p1, p2);
	Vector v2 = Vector(p1, p3);
	Vector n = v2.cross(v1).normalize();
	this->A = n.getX();
	this->B = n.getY();
	this->C = n.getZ();
	this->D = -(A*p1.getX()+B*p1.getY()+C*p1.getZ());//Ax+By+Cz+D=0
	this->pointOnPlane = p1;
}
Plane::Plane(Material material, double a, double b, double c, double d) {
	this->mat = material;
	this->A = a;
	this->B = b;
	this->C = c;
	this->D = d;
	if(this->A != 0) {//Each of these should just about never trigger due to floating point accuracy, but just in case
		this->pointOnPlane = Point3D(-(this->B + this->C + this->D) / this->A, 1, 1);
	}
	else if(this->B != 0) {
		this->pointOnPlane = Point3D(-(this->A + this->C + this->D) / this->B, 1, 1);
	}
	else if(this->C != 0) {
		this->pointOnPlane = Point3D(-(this->A + this->B + this->D) / this->C, 1, 1);
	}
	else {//A==B==C==0, so D==0 or plane is invalid. Either way, just slap (0, 0, 0) as the point the plane goes through
		this->pointOnPlane = Point3D(0, 0, 0);
	}
	//Ax+By+Cz+D=0
	//x=-(By+Cz+D)/A
}
Plane::Plane() {
	this->A = 0;
	this->B = 0;
	this->C = 0;
	this->D = 0;
	this->pointOnPlane = Point3D(0, 0, 0);
	this->mat = Material{};
}

intersectionInfoStruct Plane::getIntersection(Ray ray) {
	intersectionInfoStruct info{};
	Point3D start = ray.getStart();
	Vector dir = ray.getVector();
	Vector n = Vector(A, B, C);
	double denom = n.dot(dir);
	if (fabs(denom) < 0.000000001) {//Effectively 0
		info.t = -1;//Parallel
		return info;
	}
	double numer = -(n.dot(start) + D);
	info.t = numer / denom;
	if (info.t < 0) {
		return info;//Intersection is behind the ray start
	}
	info.point = start + info.t * dir.asPoint();
	return info;
}

Vector Plane::getNormal() const {
	return Vector(A, B, C).normalize();
}