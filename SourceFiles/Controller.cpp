#include "Controller.h"

#include <functional>
#include <iostream>
#include "util.h"

Controller::Controller(int numberOfArguments, char** arguments)
	:
	_view(numberOfArguments, arguments),
	_model()
{
	QtView& view = this->_view;
	Model& model = this->_model;
	//################################
	//### setup ui event callbacks ###
	//################################

	//function if new File loaded
	view.setOnRequestLoadFile( [this, &model, &view](std::string path)->void {

	std::shared_ptr<std::vector<Point3d>> pointData = FileLoader().load(path);
	model.addPointDataSet(pointData);
	
	std::shared_ptr<PointCloud3d> pointCloudPtr = std::make_shared<PointCloud3d>(
		this->_getStoreCreatedDataFunctor(),
		*util::computePointPtrVectorFrom(*pointData)
	);
	model.add(pointCloudPtr);
	double fieldOfViewAngleOnYAxis = 45; 
	double pointCloudRadius = pointCloudPtr->getRadius();
	double overviewDistance = pointCloudRadius / tan((3.1415 / 180.0) * (fieldOfViewAngleOnYAxis / 2));

	ProjectionModel& projectionModel = model.getProjectionModel();
	projectionModel.setFieldOfViewAngleInYDirectionTo(fieldOfViewAngleOnYAxis);
	projectionModel.setNearClippingPlaneZTo(0.001);
	projectionModel.setFarClippingPlaneZTo(overviewDistance + 3*pointCloudRadius);

	Point3d cameraPosition = pointCloudPtr->getCenter() + Point3d(0, 0, overviewDistance);
	model.getCameraModel().setWorldPositionTo(cameraPosition);

	model.setRotationAngleAroundYAxis(0);

	model.setRotationAngleAroundYAxis(0);
	view.render(model.getVisibleClouds(), model.getModelProperties());
	});
	
	view.setOnRequestPaintGL( 
		[&view, &model]()->void { 
		if (model.getNumberOfPointClouds() == 0) return;
			view.render(model.getVisibleClouds(), model.getModelProperties());
		}
	);

	view.setOnRequestSmoothCloud(
		[&view, &model]()->void {
		if (model.getNumberOfPointClouds() == 0) return;
		model.smoothVisibleClouds(0.01);
		view.render(model.getVisibleClouds(), model.getModelProperties());
	}
	);

	view.setOnRequestThinCloud(
		[&view, &model]()->void {
		if (model.getNumberOfPointClouds() == 0) return;
		view.render(model.getVisibleClouds(), model.getModelProperties());
	}
	);
	/*std::function<void(double, double)> onScroll = [&model](double offsetX, double offsetY)->void {

		double maxOffsetAccordingToObservations = 16;
		double dMax = 0.1;
		double factor = 1 + dMax*(offsetY / maxOffsetAccordingToObservations);

		double oldFieldOfViewAngle = model.getProjectionModel().getFieldOfViewAngleInYDirection();
		double newFieldOfViewAngle = oldFieldOfViewAngle * factor;

		model.getProjectionModel().setFieldOfViewAngleInYDirectionTo(newFieldOfViewAngle);
	};
        this->_view.setOnScrollCallbackTo(onScroll)

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
        this->_view.setOnKeyCallbackTo(onKey);*/
}

Controller::~Controller() {

}

int Controller::startApplication(){
	return this->_view.startApplication();
}

std::function<void(std::shared_ptr<std::vector<Point3d>>)> Controller::_getStoreCreatedDataFunctor() {

	Model & model = this->_model;
	return [&model](std::shared_ptr<std::vector<Point3d>> pointData)->void {
		model.addPointDataSet(pointData);
	};
}
