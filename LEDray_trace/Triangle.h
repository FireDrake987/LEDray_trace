#pragma once
#include "Plane.h"
#include "Point3D.h"

class Triangle : Plane {
	Point3D p1;
	Point3D p2;
	Point3D p3;

public: 
	Triangle(Point3D p1, Point3D p2, Point3D p3);
	Triangle(Triangle *copy);
	Triangle();

	intersectionInfoStruct getIntersection(Ray ray);

	static double triangleArea(Point3D p1, Point3D p2, Point3D p3);
};