#include "Camera.h"
#include "Plane.h"
#include "BGRPixel.h"
#include "Vector.h"

double const Camera::RENDER = DBL_MAX;

Camera::Camera(double x, double y, double z, double width, double height, Quaternion camRot) : width(width), height(height) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->camRot = camRot;
	this->FOVx = 90;
	this->FOVy = 75;
	this->ready = false;
}
Camera::Camera() : width(0), height(0) {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->camRot = Quaternion();
	this->FOVx = 90;
	this->FOVy = 75;
	this->ready = false;
}
void Camera::invalidate() {
	this->ready = false;
	this->output.clear();
	this->map.clear();
}
void Camera::build() {
	buildMap();
	this->output.clear();
	this->ready = true;
}
void Camera::buildMap() {
	this->map.clear();
	if(type == FLAT) {
		Vector min = angleToVector(-FOVx / 2, -FOVy / 2);
		Vector addX = angleToVector(FOVx / 2, -FOVy / 2) - min;
		Vector addY = angleToVector(-FOVx / 2, FOVy / 2) - min;;
		for (int h = 0; h < height; h++) {
			double hRatio = h / height;
			std::vector<Vector> accumulator;
			for(int w = 0; w < width; w++) {
				double wRatio = w / width;
				accumulator.push_back(min + wRatio*addX + hRatio*addY);
			}
			map.push_back(accumulator);
		}
	}
	else if(type == CURVED) {
		for (double yaw = (-FOVx / 2); yaw < FOVx / 2; yaw += (-FOVx / 2) / width) {
			for (double pitch = (-FOVy / 2); pitch < FOVy / 2; pitch += (-FOVy / 2) / height) {
				//Not doing this today sry
			}
		}
	}
}
std::vector<std::vector<BGRPixel>> Camera::render() {
	if(!ready) {
		build();
	}
	std::vector<std::vector<BGRPixel>> output;
	for(std::vector<Vector> row : map) {
		std::vector<BGRPixel> pxRow;
		for(Vector value : row) {
			Ray ray(Point3D(x, y, z), value);
			//TODO: Render scene & init scene
			double minDist = Camera::RENDER;
			Plane* minObj;
			intersectionInfoStruct info;
			for(Plane obj : scene) {
				info = obj.getIntersection(ray);
				if(info.t < 0) { continue; }
				if(info.t < minDist) {
					minDist = info.t;
					minObj = &obj;
				}
			}
			pxRow.push_back(minObj->getMaterial().getColAtPoint(info.point));
		}
		output.push_back(pxRow);
	}
	return output;
}
Vector Camera::angleToVector(double yaw, double pitch) {
	double sinyaw = sinf(yaw);
	double sinpitch = sinf(pitch);
	double cosyaw = cosf(yaw);
	double cospitch = cosf(pitch);
	return Vector(cospitch*sinyaw, -sinpitch, cospitch*cosyaw);
}