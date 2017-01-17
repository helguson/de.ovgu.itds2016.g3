#pragma once

#include "Point3d.h"
#include <qmatrix4x4.h>

class CameraModel {
public:
	CameraModel();
	~CameraModel();
	void reinitialize();
	void setCameraPositionTo(Point3d position);
	Point3d getCameraPosition();
	void setSceneCenterTo(Point3d position);
	Point3d getSceneCenter();
	void setUpVectorTo(Point3d position);
	Point3d getUpVector();
	QMatrix4x4 getViewMatrix();
	void rotate(Point3d axis, double angle);
	void translate(Point3d point);
	// TODO: getters and setters for remaining properties
private: 
	Point3d _cameraPosition;
	Point3d _sceneCenter;
	Point3d _upVector;
	QMatrix4x4 _viewMatrix;
	// TODO: Vector3d _worldViewDirection;
	// TODO: Vector3d _worldUpDirection;
	// TODO: PerspectiveIdentifier _perspective;
};