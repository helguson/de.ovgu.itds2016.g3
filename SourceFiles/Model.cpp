#include "Model.h"



Model::Model()
	:
	_pointDataSets(),
	_renderableObjects(),
	_cameraModel(),
	_projectionModel()
{
}

Model::~Model()
{
}

void Model::add(std::shared_ptr<PointCloud3d> pointCloudPtr) {
	this->_renderableObjects.push_back(pointCloudPtr);
}

void Model::add(std::shared_ptr<BestFitLine> bfLinePtr) {
	this->_renderableObjects.push_back(bfLinePtr);
}

void Model::add(std::shared_ptr<BestFitPlane> bfPlanePtr) {
	this->_renderableObjects.push_back(bfPlanePtr);
}

void Model::addNormals(std::shared_ptr<PointCloud3d>& pointCloudPtr, double radius)
{
	std::shared_ptr<std::vector<Point3d>> normals = computeNormalVectors(pointCloudPtr, radius);
	this->_normalVectorSets.push_back(normals);
	pointCloudPtr->setNormalDataTo(this->_normalVectorSets.back());
}
void Model::add(std::shared_ptr<BestFitSphere> bfSpherePtr) {
	this->_renderableObjects.push_back(bfSpherePtr);
}

void Model::addPointDataSet(std::shared_ptr<std::vector<Point3d>> pointDataSet) {
	this->_pointDataSets.push_back(pointDataSet);
}

size_t Model::getNumberOfRenderableObjects() const {
	return this->_renderableObjects.size();
}

std::shared_ptr<RenderableObjects> Model::getRenderableObjectAt(int index) {
	return this->_renderableObjects.at(index);
}


double Model::getFieldOfViewAngleInYDirection()
{
	return this->_projectionModel.getFieldOfViewAngleInYDirection();
}

Point3d Model::getSceneCenter()
{
	return this->_cameraModel.getSceneCenter();
}

Point3d Model::getCameraPosition()
{
	return this->_cameraModel.getCameraPosition();
}

QMatrix4x4 Model::getModelViewProjectionMatrix() {
	return this->_projectionModel.getProjektionMatrix()*this->_cameraModel.getViewMatrix()*this->_modelMatrix;
}

QMatrix4x4 Model::getModelViewMatrix() {
	return this->_cameraModel.getViewMatrix()*this->_modelMatrix;
}

void Model::reinitializeCamera()
{
	this->_cameraModel.reinitialize();
}

void Model::setCameraPositionTo(Point3d position)
{
	this->_cameraModel.setCameraPositionTo(position);
}

void Model::setFieldOfViewAngleInYDirectionTo(double angle)
{
	this->_projectionModel.setFieldOfViewAngleInYDirectionTo(angle);
}

void Model::setNearClippingPlaneZTo(double z)
{
	this->_projectionModel.setNearClippingPlaneZTo(z);
}

void Model::setFarClippingPlaneZTo(double z)
{
	this->_projectionModel.setFarClippingPlaneZTo(z);
}

void Model::setWindowSize(double width, double height)
{
	this->_projectionModel.setWindowSize(width, height);
}

void Model::setSceneCenterTo(Point3d position)
{
	this->_cameraModel.setSceneCenterTo(position);
}

void Model::rotateCamera(Point3d axis, double angle)
{
	this->_cameraModel.rotate(axis, angle);
}

void Model::translateCamera(Point3d axis)
{
	this->_cameraModel.translate(axis);
}

bool Model::smoothVisibleCloud(int index, double smoothFactor) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		this->add(cloudPtr->computeSmoothedVersionWith(smoothFactor));
		return true;
	}
	return false;
}

bool Model::thinVisibleCloud(int index, double thinningRadius) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		this->add(cloudPtr->computeThinnedVersionWith(thinningRadius));
		return true;
	}
	return false;
}

bool Model::bfLine(int index) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		std::shared_ptr<BestFitLine> line = std::make_shared<BestFitLine>(*cloudPtr);
		this->add(line);
		return true;
	}
	return false;
}

bool Model::bfPlane(int index) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		std::shared_ptr<BestFitPlane> plane = std::make_shared<BestFitPlane>(*cloudPtr);
		this->add(plane);
		return true;
	}
	return false;
}

bool Model::bfSphere(int index) {
	std::shared_ptr<PointCloud3d> cloudPtr = std::dynamic_pointer_cast<PointCloud3d>(this->_renderableObjects.at(index));
	if (cloudPtr) {
		std::shared_ptr<BestFitSphere> sphere = std::make_shared<BestFitSphere>(*cloudPtr);
		this->add(sphere);
		return true;
	}
	return false;
}
