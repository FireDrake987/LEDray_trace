#pragma once
#include "Quaternion.h"

class Camera {
	double x, y, z;
	double width, height;
	Quaternion camRot;
	Point3D** map;
	bool ready;

public: 
	Camera(double x, double y, double z, double width, double height, Quaternion camRot);
	Camera();
};