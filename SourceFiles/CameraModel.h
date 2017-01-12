#pragma once

#include "Point3d.h"
#include <qmatrix4x4.h>

class CameraModel {
public:
	CameraModel();
	~CameraModel();
	void setWorldPositionTo(Point3d position);
	Point3d getWorldPosition();
	void setSceneCenterTo(Point3d position);
	Point3d getSceneCenter();
	void setUpVectorTo(Point3d position);
	Point3d getUpVector();
	QMatrix4x4 CameraModel::getCameraTransformation();
	void rotate(Point3d axis, double angle);
	void translate(Point3d point);
	// TODO: getters and setters for remaining properties
private: 
	Point3d _worldPosition;
	Point3d _sceneCenter;
	Point3d _upVector;
	QMatrix4x4 _sceneTransformationMatrix;
	// TODO: Vector3d _worldViewDirection;
	// TODO: Vector3d _worldUpDirection;
	// TODO: PerspectiveIdentifier _perspective;
};