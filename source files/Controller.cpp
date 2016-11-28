#include "Controller.h"

#include <functional>
#include <iostream>
#include "util.h"

Controller::Controller()
	:
	_view(),
	_model(),
	_fileLoader(),
	_shouldContiniueLooping(false)
{
	//###################
	//### setup model ###
	//###################


	std::shared_ptr<std::vector<Point3d>> pointData = this->_fileLoader.load();
	this->_model.addPointDataSet(pointData);
	
	std::shared_ptr<PointCloud3d> pointCloudPtr = std::make_shared<PointCloud3d>(
		this->_getStoreCreatedDataFunctor(),
		*util::computePointPtrVectorFrom(*pointData)
	);
	this->_model.add(pointCloudPtr);
	double fieldOfViewAngleOnYAxis = 45; 
	double pointCloudRadius = pointCloudPtr->getRadius();
	double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

	ProjectionModel& projectionModel = this->_model.getProjectionModel();
	projectionModel.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
	projectionModel.setNearClippingPlaneZTo(0.001);
	projectionModel.setFarClippingPlaneZTo(overviewDistance + 3*pointCloudRadius);

	Point3d cameraPosition = pointCloudPtr->getCenter() + Point3d(0, 0, overviewDistance);
	this->_model.getCameraModel().setWorldPositionTo(cameraPosition);

	this->_model.setRotationAngleAroundYAxis(0);

	//################################
	//### setup ui event callbacks ###
	//################################
	Model& model = this->_model;

	std::function<void()> onDemandClosingOfWindow = [this]()->void {
		this->stopMainLoop();
	};
	this->_view.setOnDemandClosingOfWindowCallbackTo(onDemandClosingOfWindow);

	std::function<void(double, double)> onScroll = [&model](double offsetX, double offsetY)->void {

		double maxOffsetAccordingToObservations = 16;
		double dMax = 0.1;
		double factor = 1 + dMax*(offsetY / maxOffsetAccordingToObservations);

		double oldFieldOfViewAngle = model.getProjectionModel().getFieldOfViewAngleInYDirection();
		double newFieldOfViewAngle = oldFieldOfViewAngle * factor;

		model.getProjectionModel().setFieldOfViewAngleInYDirectionTo(newFieldOfViewAngle);
	};
	this->_view.setOnScrollCallbackTo(onScroll);

	std::function<void(GLFWwindow*, int, int, int, int)> onKey = [&model](GLFWwindow* windowPtr, int key, int scancode, int action, int mods)->void {

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			
			if (key == GLFW_KEY_LEFT) {
				double oldAngle = model.getRotationAngleAroundYAxis();
				model.setRotationAngleAroundYAxis(oldAngle + 1);
			}

			if (key == GLFW_KEY_RIGHT) {
				double oldAngle = model.getRotationAngleAroundYAxis();
				model.setRotationAngleAroundYAxis(oldAngle - 1);
			}
		}
	};
	this->_view.setOnKeyCallbackTo(onKey);
}

Controller::~Controller() {

}

std::function<void(std::shared_ptr<std::vector<Point3d>>)> Controller::_getStoreCreatedDataFunctor() {

	Model & model = this->_model;
	return [&model](std::shared_ptr<std::vector<Point3d>> pointData)->void {
		model.addPointDataSet(pointData);
	};
}

void Controller::startMainLoop() {

	this->_shouldContiniueLooping = true;
	while (this->shouldDoNextLoop()) {

		this->_view.render(
			this->_model.getPointCloudAt(0),
			this->_model.getCameraModel(),
			this->_model.getProjectionModel(),
			this->_model.getRotationAngleAroundYAxis()
		);
	}
}

void Controller::stopMainLoop() {

	this->_shouldContiniueLooping = false;
}

bool Controller::shouldDoNextLoop() {

	return this->_shouldContiniueLooping;
}