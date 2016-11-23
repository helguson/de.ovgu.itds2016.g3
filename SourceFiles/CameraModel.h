#pragma once

#include "Point3d.h"

class CameraModel {
public:
	CameraModel();
	~CameraModel();
	void setWorldPositionTo(Point3d position);
	Point3d getWorldPosition();
	// TODO: getters and setters for remaining properties
private: 
	Point3d _worldPosition;
	// TODO: Vector3d _worldViewDirection;
	// TODO: Vector3d _worldUpDirection;
	// TODO: PerspectiveIdentifier _perspective;
};