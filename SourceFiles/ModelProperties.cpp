#include "ModelProperties.h"

ModelProperties::ModelProperties()
	:
	_fieldOfViewAngleInYDirection(),
	_farClippingPlaneZ(),
	_nearClippingPlaneZ(),
	_worldPosition(),
	_sceneCenter()
{
}

ModelProperties::ModelProperties(double fieldOfViewAngleInYDirection, double farClippingPlaneZ,	double nearClippingPlaneZ, Point3d worldPosition, Point3d sceneCenter)
	:
	_fieldOfViewAngleInYDirection(fieldOfViewAngleInYDirection),
	_farClippingPlaneZ(farClippingPlaneZ),
	_nearClippingPlaneZ(nearClippingPlaneZ),
	_worldPosition(worldPosition),
	_sceneCenter(sceneCenter)
{
}


ModelProperties::~ModelProperties()
{
}
