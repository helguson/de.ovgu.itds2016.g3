#include "CameraModel.h"

CameraModel::CameraModel()
	:
	_cameraPosition(),
	_sceneCenter(),
	_upVector(0,1,0),
	_viewMatrix()
{
}

CameraModel::~CameraModel() {

}

void CameraModel::reinitialize() {
	this->_viewMatrix.setToIdentity();
	QVector3D camera(this->_cameraPosition.x, this->_cameraPosition.y, this->_cameraPosition.z);
	QVector3D scene(this->_sceneCenter.x, this->_sceneCenter.y, this->_sceneCenter.z);
	QVector3D upVec(this->_upVector.x, this->_upVector.y, this->_upVector.z);
	this->_viewMatrix.lookAt(camera, scene, upVec);
}

void CameraModel::setCameraPositionTo(Point3d position) {

	this->_cameraPosition = position;
}

Point3d CameraModel::getCameraPosition() {
	
	return this->_cameraPosition;
}

void CameraModel::setSceneCenterTo(Point3d position) {

	this->_sceneCenter = position;
}

Point3d CameraModel::getSceneCenter() {

	return this->_sceneCenter;
}

void CameraModel::setUpVectorTo(Point3d upVec)
{
	this->_upVector = upVec;
}

Point3d CameraModel::getUpVector()
{
	return this->_upVector;
}

QMatrix4x4 CameraModel::getViewMatrix()
{
	return this->_viewMatrix;
}

void CameraModel::rotate(Point3d axis, double angle)
{
	this->_viewMatrix.translate(QVector3D(this->_sceneCenter.x, this->_sceneCenter.y, this->_sceneCenter.z));
	this->_viewMatrix.rotate(2 * angle * 180.0 / 3.1415, QVector3D(axis.x, axis.y, axis.z));
	this->_viewMatrix.translate(QVector3D(-this->_sceneCenter.x, -this->_sceneCenter.y, -this->_sceneCenter.z));
}

void CameraModel::translate(Point3d point)
{
	this->_viewMatrix.translate(QVector3D(point.x, point.y, point.z));
}


