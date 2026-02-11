#include "Camera.h"
#include "Plane.h"
#include "BGRPixel.h"
#include "Vector.h"

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
std::vector<std::vector<BGRPixel>> Camera::render(int x1, int y1, int x2, int y2) {
	if(!ready) {
		build();
	}
	std::vector<std::vector<BGRPixel>> output;
	for(int i = y1; i < y2; i++) {
		std::vector<Vector> row = map.at(i);
		std::vector<BGRPixel> pxRow;
		for(int j = x1; j < x2; j++) {
			Vector value = row.at(j);
			Ray ray(Point3D(x, y, z), value);
			double minDist = Camera::RENDER;
			Plane minPlane = Plane();
			Plane* minObj = &minPlane;
			intersectionInfoStruct info;
			for(Plane obj : scene) {
				info = obj.getIntersection(ray);
				if(info.t < 0) { continue; }
				if(info.t < minDist) {
					minDist = info.t;
					minObj = &obj;
				}
			}
			if (minDist < Camera::RENDER * 0.9999999999) {
				pxRow.push_back(minObj->getMaterial().getColAtPoint(info.point));
			}
			pxRow.push_back(Camera::DEFAULT_COLOR);
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