#include "Ray.h"

Ray::Ray(Point3D origin, Vector direction) {
	this->start = origin;
	this->dir = direction.normalize();
}
Ray::Ray(Vector direction) {
	this->start = Point3D();
	this->dir = direction.normalize();
}
