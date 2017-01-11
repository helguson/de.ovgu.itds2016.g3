#include "ProjectionModel.h"

ProjectionModel::ProjectionModel(double fieldOfViewAngleInYDirection, double nearClippingPlaneZ, double farClippingPlaneZ)
	:
	_fieldOfViewAngleInYDirection(fieldOfViewAngleInYDirection),
	_nearClippingPlaneZ(nearClippingPlaneZ),
	_farClippingPlaneZ(farClippingPlaneZ),
	_width(0),
	_height(0)
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
double ProjectionModel::getAspectRatio()
{
	return this->_width/this->_height;
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

void ProjectionModel::setWindowSize(double width, double height) {
	this->_width = width;
	this->_height = height;
}

QMatrix4x4 ProjectionModel::getProjektionMatrix()
{
	QMatrix4x4 tmp;
	tmp.perspective(this->_fieldOfViewAngleInYDirection, this->_width/this->_height, this->_nearClippingPlaneZ, this->_farClippingPlaneZ);
	tmp.viewport(0, 0, _width, _height);
	return tmp;
}
