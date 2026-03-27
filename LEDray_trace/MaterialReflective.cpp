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
BGRPixel MaterialReflective::getColAtPoint(Point3D intPoint, Camera* cam, Ray &ray, Plane *plane, int str) const {
	BGRPixel baseCol = Material::getColAtPoint(intPoint, cam, ray, plane, str);
	Vector normal = plane->getNormal();
	Vector rayDir = ray.getVector().normalize();
	if (str < 0.1) {
		return baseCol;
	}
	Vector reflectDir = rayDir - 2 * Vector::dot(rayDir, normal) * normal;
	Ray reflectRay(intPoint + (0.00001 * reflectDir).asPoint(), reflectDir);
	BGRPixel reflectCol = cam->traceRay(reflectRay, str);
	return BGRPixel{
		static_cast<uint8_t>(baseCol.b * (1 - reflectance) + reflectCol.b * reflectance),
		static_cast<uint8_t>(baseCol.g * (1 - reflectance) + reflectCol.g * reflectance),
		static_cast<uint8_t>(baseCol.r * (1 - reflectance) + reflectCol.r * reflectance)
	};
}