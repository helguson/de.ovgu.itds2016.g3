#pragma once

#include "Point3d.h"

class CameraModel {
public:
	CameraModel();
	~CameraModel();
	void setWorldPositionTo(Point3d position);
	Point3d getWorldPosition();
	void setSceneCenterTo(Point3d position);
	Point3d getSceneCenter();
	// TODO: getters and setters for remaining properties
private: 
	Point3d _worldPosition;
	Point3d _sceneCenter;
	// TODO: Vector3d _worldViewDirection;
	// TODO: Vector3d _worldUpDirection;
	// TODO: PerspectiveIdentifier _perspective;
};