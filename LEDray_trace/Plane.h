#pragma once
#include "intersectionInfoStruct.h"
#include "Ray.h"

class Plane {
	double A, B, C, D;//Ax+By+Cz+D=0
	Point3D pointOnPlane;

protected:
	Material mat;

public:
	Plane(Material mat, Point3D p1, Point3D p2, Point3D p3);
	Plane(Material mat, double a, double b, double c, double d);
	Plane(Plane& copy);
	Plane();

	intersectionInfoStruct getIntersection(Ray ray);

	inline double getA() const { return A; }
	inline double getB() const { return B; }
	inline double getC() const { return C; }
	inline double getD() const { return D; }
	inline Material getMaterial() const { return mat; }
	inline Point3D getPointOnPlane() const { return pointOnPlane; }
};