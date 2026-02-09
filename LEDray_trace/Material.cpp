#include "pch.h"
#include "Material.h"

Material::Material(BGRPixel col, double luminance, double reflectance, double specularity, double absorbance, double trasmittance, double refractiveIndex) {
	this->col = col;
	this->luminance = luminance;
	this->reflectance = reflectance;
	this->specularity = specularity;
	this->absorbance = absorbance;
	this->transmittance = trasmittance;
	this->refractiveIndex = refractiveIndex;
}
Material::Material(BGRPixel col) : Material(col, 0, 0, 1, 1, 0, 1) {}
Material::Material() : Material(BGRPixel{}) {}