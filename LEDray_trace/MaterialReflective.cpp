#include "Material.h"
#include "MaterialReflective.h"
#include "Camera.h"
#include "Plane.h"

MaterialReflective::MaterialReflective(double reflectance, BGRPixel col) : Material(col) {
	this->reflectance = reflectance;
}
MaterialReflective::MaterialReflective(BGRPixel col) : Material(col) {
	reflectance = 0.1;
}
MaterialReflective::MaterialReflective() : Material() {
	reflectance = 0.1;
}
BGRPixel MaterialReflective::getColAtPoint(Point3D intPoint, Camera* cam, Ray &ray, Plane *plane) const {
	BGRPixel baseCol = Material::getColAtPoint(intPoint, cam, ray, plane);
	Vector normal = plane->getNormal();
	Vector rayDir = ray.getVector().normalize();
	Vector reflectDir = rayDir - 2 * Vector::dot(rayDir, normal) * normal;
	Ray reflectRay(intPoint, reflectDir);
	BGRPixel reflectCol = cam->traceRay(reflectRay);
	return BGRPixel{
		static_cast<uint8_t>(baseCol.b * (1 - reflectance) + reflectCol.b * reflectance),
		static_cast<uint8_t>(baseCol.g * (1 - reflectance) + reflectCol.g * reflectance),
		static_cast<uint8_t>(baseCol.r * (1 - reflectance) + reflectCol.r * reflectance)
	};
}