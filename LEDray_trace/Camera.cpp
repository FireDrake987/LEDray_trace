#include "Camera.h"
#include "Plane.h"
#include "BGRPixel.h"
#include "Vector.h"

double const PI = 3.14159265358979323846264338327950288;//I think thats enough precision

Camera::Camera(double x, double y, double z, double width, double height, Quaternion camRot) : width(width), height(height) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->camRot = camRot;
	this->FOVx = PI/2;
	this->FOVy = 5*PI/12;
	this->ready = false;
}
Camera::Camera() : width(0), height(0) {
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->camRot = Quaternion();
	this->FOVx = PI/2;
	this->FOVy = 5*PI/12;
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
		Vector min = angleToVector(-FOVx / 2, -FOVy / 2).normalize();
		Vector addX = (angleToVector(FOVx / 2, -FOVy / 2) - min).normalize();
		Vector addY = (angleToVector(-FOVx / 2, FOVy / 2) - min).normalize();
		for (int h = 0; h < height; h++) {
			double hRatio = (1.0*h) / (height-1);
			std::vector<Vector> accumulator;
			for(int w = 0; w < width; w++) {
				double wRatio = (1.0*w) / (width-1);
				Vector result = min + addX * wRatio + addY * hRatio;
				result = camRot.apply(result.asPoint());
				accumulator.push_back(result.normalize());
			}
			map.push_back(accumulator);
		}
	}
	else if (type == CURVED) {
		for (double pitch = (-FOVy / 2); pitch < FOVy / 2; pitch += FOVy / height) {
			std::vector<Vector> accumulator;
			for (double yaw = (-FOVx / 2); yaw < FOVx / 2; yaw += FOVx / width) {
				Vector result = angleToVector(yaw, pitch).normalize();
				result = camRot.apply(result.asPoint());
				accumulator.push_back(result);
			}
			map.push_back(accumulator);
		}
	}
}
void Camera::eulerRotate(double yaw, double pitch) {
	Vector yawAxis = Vector(0, 1, 0);
	Vector pitchAxis = Vector(1, 0, 0);
	yawAxis = camRot.apply(yawAxis.asPoint());
	Quaternion yawQ = Quaternion(cos(yaw / 2), yawAxis * sin(yaw / 2));
	pitchAxis = camRot.apply(pitchAxis.asPoint());
	Quaternion pitchQ = Quaternion(cos(pitch / 2), pitchAxis * sin(pitch / 2));
	camRot = yawQ * pitchQ * camRot;
	camRot = camRot.normalize();
}

void Camera::eulerRotate(double yaw, double pitch, double roll) {
	Vector yawAxis = Vector(0, 1, 0);
	Vector pitchAxis = Vector(1, 0, 0);
	Vector rollAxis = Vector(0, 0, 1);
	yawAxis = camRot.apply(yawAxis.asPoint());
	Quaternion yawQ = Quaternion(cos(yaw / 2), yawAxis * sin(yaw / 2));
	pitchAxis = camRot.apply(pitchAxis.asPoint());
	Quaternion pitchQ = Quaternion(cos(pitch / 2), pitchAxis * sin(pitch / 2));
	rollAxis = camRot.apply(rollAxis.asPoint());
	Quaternion rollQ = Quaternion(cos(roll / 2), rollAxis * sin(roll / 2));
	camRot = yawQ * pitchQ * rollQ * camRot;
	camRot = camRot.normalize();
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
			Ray ray = Ray(Point3D(x, y, z), value);
			Plane* minObj = nullptr;
			intersectionInfoStruct minInfo;
			minInfo.t = Camera::RENDER;
			for(Plane *obj : scene) {
				intersectionInfoStruct info = obj->getIntersection(ray);
				if(info.t < 0) { continue; }
				if(info.t < minInfo.t) {
					minInfo = info;
					minObj = obj;
				}
			}
			if (minObj && minInfo.t < Camera::RENDER * 0.9999999999) {
				pxRow.push_back(minObj->getMaterial().getColAtPoint(minInfo.point));
			}
			else {
				pxRow.push_back(Camera::DEFAULT_COLOR);
			}
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
	return Vector(cospitch*sinyaw, sinpitch, cospitch*cosyaw);
}