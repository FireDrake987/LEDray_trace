#include "Triangle.h"

Triangle::Triangle(Material material, Point3D p1, Point3D p2, Point3D p3) : Plane(material, p1, p2, p3) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}
Triangle::Triangle() : Plane() {
	this->p1 = this->p2 = this->p3 = Point3D();
}
intersectionInfoStruct Triangle::getIntersection(Ray ray) {
	intersectionInfoStruct info = Plane::getIntersection(ray);
	Point3D p = info.point;
	double sub1 = triangleArea(p, p1, p2);
	double sub2 = triangleArea(p, p2, p3);
	double sub3 = triangleArea(p, p1, p3);
	double area = triangleArea(p1, p2, p3);
	if(sub1 + sub2 + sub3 > area + 0.000000001) {
		//Point outside triangle
		info.t = -1;
		return info;
	}
	return info;//Point inside triangle, we are fine
}
double Triangle::triangleArea(Point3D p1, Point3D p2, Point3D p3) {
	Vector v1(p1, p2);
	Vector v2(p1, p3);
	Vector n = v1.cross(v2);
	double area = n.magnitude() / 2;
	return area;
}