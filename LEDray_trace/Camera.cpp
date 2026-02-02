#include "pch.h"
#include "Camera.h"

Camera::Camera(double x, double y, double z, double width, double height, Quaternion camRot) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->width = width;
	this->height = height;
	this->camRot = camRot;
	this->ready = false;
}
Camera::Camera() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->width = 0;
	this->height = 0;
	this->camRot = Quaternion();
	this->ready = false;
}
