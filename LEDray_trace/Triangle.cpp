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
    if (info.t < 0) return info;

    Vector v0(p1, p2);
    Vector v1(p1, p3);
    Vector v2(p1, info.point);

    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v0);
    double d21 = v2.dot(v1);

    double denom = d00 * d11 - d01 * d01;
    if (fabs(denom) < 1e-12) { info.t = -1; return info; }

    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;

    const double epsilon = -1e-6;
    if(u < epsilon || v < epsilon || w < epsilon) {
        if(u > 1.0 + epsilon || v > 1.0 + epsilon || w > 1.0 + epsilon) {
            info.t = -1;
        }
    }
    return info;
}
double Triangle::triangleArea(Point3D p1, Point3D p2, Point3D p3) {
	Vector v1(p1, p2);
	Vector v2(p1, p3);
	Vector n = v1.cross(v2);
	double area = n.magnitude() / 2;
	return area;
}