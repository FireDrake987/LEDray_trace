#pragma once
#include "Quaternion.h"
#include "BGRPixel.h"
#include <vector>

class Camera {
	double x, y, z;
	double width, height;
	double FOVx, FOVy;
	Quaternion camRot;
	std::vector<std::vector<Vector>> map;
	std::vector<std::vector<BGRPixel>> output;
	std::vector<Plane> scene;
	bool ready;

public:
	static enum CameraType {
		FLAT, CURVED
	};
	inline static CameraType type = FLAT;

	Camera(double x, double y, double z, double width, double height, Quaternion camRot);
	Camera();

	inline void rawMove(double dx, double dy, double dz) {
		x += dx;
		y += dy;
		z += dz;
	}
	void move(Vector dir);//Respecting camRot
	void move(double right, double down, double forwards);//Same thing as vector form
	inline void setRot(Quaternion newRot) {
		camRot = newRot;
		invalidate();
	}
	inline void rotate(Quaternion rot) {
		camRot = camRot * rot;
		invalidate();
	}
	void invalidate();//Sets ready=false, clears output and map
	void build();//Rebuild Camera based on current data, sets ready=true
	void buildMap();//Initializes map to proper mapping based on current configurations, called by regenerate

	//Automatically calls build() when ready!=true
	std::vector<std::vector<BGRPixel>> render();//TODO: Finish function header

	Vector angleToVector(double yaw, double pitch);
};