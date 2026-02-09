#include "Ray.h"

Ray::Ray(Point3D origin, Vector direction) {
	this->start = origin;
	this->dir = direction;
}
Ray::Ray(Vector direction) {
	this->start = Point3D();
	this->dir = direction;
}
