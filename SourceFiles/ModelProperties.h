#pragma once
#include "Point3d.h"

class ModelProperties
{
public:
	ModelProperties();
	ModelProperties(
		double fieldOfViewAngleInYDirection,
		double farClippingPlaneZ, 
		double nearClippingPlaneZ,
		double rotationAngle,
		Point3d worldPosition,
		Point3d sceneCenter);

	~ModelProperties();

	double _fieldOfViewAngleInYDirection;
	double _farClippingPlaneZ;
	double _nearClippingPlaneZ;
	double _rotationAngle;
	Point3d _worldPosition;
	Point3d _sceneCenter;
};

