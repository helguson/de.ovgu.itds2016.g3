#include "ProjectionModel.h"

ProjectionModel::ProjectionModel(double fieldOfViewAngleInYDirection, double nearClippingPlaneZ, double farClippingPlaneZ)
	:
	_fieldOfViewAngleInYDirection(fieldOfViewAngleInYDirection),
	_nearClippingPlaneZ(nearClippingPlaneZ),
	_farClippingPlaneZ(farClippingPlaneZ)
{
}

double ProjectionModel::getFieldOfViewAngleInYDirection() {
	return this->_fieldOfViewAngleInYDirection;
}
double ProjectionModel::getNearClippingPlaneZ() {
	return this->_nearClippingPlaneZ;
}
double ProjectionModel::getFarClippingPlaneZ() {
	return this->_farClippingPlaneZ;
}
double ProjectionModel::setFieldOfViewAngleInYDirectionTo(double angle) {
	this->_fieldOfViewAngleInYDirection = angle;
}
double ProjectionModel::setNearClippingPlaneZTo(double z) {
	this->_nearClippingPlaneZ = z;
}
double ProjectionModel::setFarClippingPlaneZTo(double z) {
	this->_farClippingPlaneZ = z;
}