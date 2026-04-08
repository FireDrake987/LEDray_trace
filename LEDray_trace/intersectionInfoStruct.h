#pragma once
#include "Point3D.h"

//Just what the name says, a struct with intersection info. This is used as the return type for the getIntersection functions of Plane and Triangle, and contains the parameter t at which the ray intersects the plane/triangle, as well as the actual intersection point in 3D space. If there is no intersection, t < 0, and the point may not be valid (e.g., it may be (0, 0, 0), uninitialized, or some other random value).
struct intersectionInfoStruct {
	double t;
	Point3D point;
};
