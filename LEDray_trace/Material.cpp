#include "Material.h"
#include "BGRPixel.h"
#include "Plane.h"

Material::Material(BGRPixel col) {
	this->col = col;
}
Material::Material() : Material(BGRPixel()) {}