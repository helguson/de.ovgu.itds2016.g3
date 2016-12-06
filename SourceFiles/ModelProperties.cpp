#include "ModelProperties.h"

ModelProperties::ModelProperties()
	:
	_fieldOfViewAngleInYDirection(),
	_farClippingPlaneZ(),
	_nearClippingPlaneZ(),
	_rotationAngle(0),
	_worldPosition(),
	_sceneCenter()
{
}

ModelProperties::ModelProperties(double fieldOfViewAngleInYDirection, double farClippingPlaneZ,	double nearClippingPlaneZ, double rotationAngle, Point3d worldPosition, Point3d sceneCenter)
	:
	_fieldOfViewAngleInYDirection(fieldOfViewAngleInYDirection),
	_farClippingPlaneZ(farClippingPlaneZ),
	_nearClippingPlaneZ(nearClippingPlaneZ),
	_rotationAngle(rotationAngle),
	_worldPosition(worldPosition),
	_sceneCenter(sceneCenter)
{
}


ModelProperties::~ModelProperties()
{
}
