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
void ProjectionModel::setFieldOfViewAngleInYDirectionTo(double angle) {
	this->_fieldOfViewAngleInYDirection = angle;
}
void ProjectionModel::setNearClippingPlaneZTo(double z) {
	this->_nearClippingPlaneZ = z;
}
void ProjectionModel::setFarClippingPlaneZTo(double z) {
	this->_farClippingPlaneZ = z;
}