#include "CameraModel.h"

CameraModel::CameraModel()
	:
	_worldPosition(),
	_sceneCenter(),
	_upVector(0,1,0),
	_sceneTransformationMatrix()
{
}

CameraModel::~CameraModel() {

}

void CameraModel::setWorldPositionTo(Point3d position) {

	this->_worldPosition = position;
}

Point3d CameraModel::getWorldPosition() {
	
	return this->_worldPosition;
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

QMatrix4x4 CameraModel::getCameraTransformation()
{
		QMatrix4x4 tmp;
		QVector3D camera(this->_worldPosition.x, this->_worldPosition.y, this->_worldPosition.z);
		QVector3D scene(this->_sceneCenter.x, this->_sceneCenter.y, this->_sceneCenter.z);
		QVector3D upVec(this->_upVector.x, this->_upVector.y, this->_upVector.z);
		tmp.lookAt(camera, scene, upVec);
		return this->_sceneTransformationMatrix*tmp;
}

void CameraModel::rotate(Point3d axis, double angle)
{
	this->_sceneTransformationMatrix.translate(QVector3D(this->_sceneCenter.x, this->_sceneCenter.y, this->_sceneCenter.z));
	this->_sceneTransformationMatrix.rotate(2 * angle * 180.0 / 3.1415, QVector3D(axis.x, axis.y, axis.z));
	this->_sceneTransformationMatrix.translate(QVector3D(-this->_sceneCenter.x, -this->_sceneCenter.y, -this->_sceneCenter.z));
}

void CameraModel::translate(Point3d point)
{
	this->_sceneTransformationMatrix.translate(QVector3D(point.x, point.y, point.z));
}


