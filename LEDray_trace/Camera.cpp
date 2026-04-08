#include "Camera.h"
#include "Plane.h"
#include "BGRPixel.h"
#include "Vector.h"
#include <mutex>
#include <shared_mutex>

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
}
void Camera::move(Vector dir) {
	Camera::move(dir.getX(), dir.getY(), dir.getZ());
}
void Camera::move(double right, double down, double forwards) {
	std::unique_lock<std::shared_mutex> lock(invalidateMut);
	Vector rightVec = camRot.apply(Vector(1, 0, 0));
	Vector downVec = camRot.apply(Vector(0, 1, 0));
	Vector forwardVec = camRot.apply(Vector(0, 0, 1));
	this->x += rightVec.getX() * right + downVec.getX() * down + forwardVec.getX() * forwards;
	this->y += rightVec.getY() * right + downVec.getY() * down + forwardVec.getY() * forwards;
	this->z += rightVec.getZ() * right + downVec.getZ() * down + forwardVec.getZ() * forwards;
	invalidate();
}
void Camera::build() {
	std::unique_lock<std::shared_mutex> lock(invalidateMut);
	if(ready) {return;}
	buildMap();
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
				result = camRot.apply(result);
				accumulator.push_back(result.normalize());
			}
			map.push_back(accumulator);
		}
	}
	else if(type == CURVED) {
		for(double pitch = (-FOVy / 2); pitch < FOVy / 2; pitch += FOVy / height) {
			std::vector<Vector> accumulator;
			for(double yaw = (-FOVx / 2); yaw < FOVx / 2; yaw += FOVx / width) {
				Vector result = angleToVector(yaw, pitch).normalize();
				result = camRot.apply(result);
				accumulator.push_back(result);
			}
			map.push_back(accumulator);
		}
	}
}
void Camera::eulerRotate(double yaw, double pitch) {
	std::unique_lock<std::shared_mutex> lock(invalidateMut);
	Vector yawAxis = Vector(0, 1, 0);
	Vector pitchAxis = Vector(1, 0, 0);
	//yawAxis = camRot.apply(yawAxis);//Disabled in order to prevent roll drift in function for fps style camera controls
	Quaternion yawQ = Quaternion(cos(yaw / 2), yawAxis * sin(yaw / 2));
	pitchAxis = camRot.apply(pitchAxis);
	Quaternion pitchQ = Quaternion(cos(pitch / 2), pitchAxis * sin(pitch / 2));
	camRot = yawQ * pitchQ * camRot;
	camRot = camRot.normalize();
	invalidate();
}

void Camera::eulerRotate(double yaw, double pitch, double roll) {
	std::unique_lock<std::shared_mutex> lock(invalidateMut);
	Vector yawAxis = Vector(0, 1, 0);
	Vector pitchAxis = Vector(1, 0, 0);
	Vector rollAxis = Vector(0, 0, 1);
	yawAxis = camRot.apply(yawAxis);
	Quaternion yawQ = Quaternion(cos(yaw / 2), yawAxis * sin(yaw / 2));
	pitchAxis = camRot.apply(pitchAxis);
	Quaternion pitchQ = Quaternion(cos(pitch / 2), pitchAxis * sin(pitch / 2));
	rollAxis = camRot.apply(rollAxis);
	Quaternion rollQ = Quaternion(cos(roll / 2), rollAxis * sin(roll / 2));
	camRot = yawQ * pitchQ * rollQ * camRot;
	camRot = camRot.normalize();
	invalidate();
}

std::vector<BGRPixel> Camera::render(int x1, int y1, int x2, int y2) {
	if(!ready) {
		build();
	}
	std::shared_lock<std::shared_mutex> lock(invalidateMut);
	std::vector<BGRPixel> output;
	output.reserve((x2-x1) * (y2-y1));
	for(int i = y1; i < y2; i++) {
		std::vector<Vector> row = map.at(i);
		for(int j = x1; j < x2; j++) {
			Vector value = row.at(j);
			Ray ray = Ray(Point3D(x, y, z), value);
			output.push_back(traceRay(ray, 1));
		}
	}
	return output;
}

BGRPixel Camera::traceRay(Ray ray, double str) {
	if(str < Camera::minStr) {
		return Camera::DEFAULT_COLOR;
	}
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
	if(minObj && minInfo.t < Camera::RENDER) {
		return minObj->getMaterial().getColAtPoint(minInfo.point, this, ray, minObj, str);
	}
	else {
		return Camera::DEFAULT_COLOR;
	}
}

Vector Camera::angleToVector(double yaw, double pitch) {
	double sinyaw = sinf(yaw);
	double sinpitch = sinf(pitch);
	double cosyaw = cosf(yaw);
	double cospitch = cosf(pitch);
	return Vector(cospitch*sinyaw, sinpitch, cospitch*cosyaw);
}