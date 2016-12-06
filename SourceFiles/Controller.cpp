#include "Controller.h"

#include <functional>
#include <iostream>
#include "util.h"

Controller::Controller(int numberOfArguments, char** arguments)
	:
	_view(numberOfArguments, arguments),
	_model()
{
	//Initialize Model and View
	QtView& view = this->_view;
	Model& model = this->_model;
	model.setFieldOfViewAngleInYDirectionTo(45);
	model.setNearClippingPlaneZTo(0.001);
	model.setRotationAngleAroundYAxis(0);

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
	view.addVisibleElementToList();
	this->_updateModelProperties();
	view.updateProjectionModelView(model.getModelProperties());
	});
	
	//function for rendering loop
	view.setOnRequestPaintGL( 
		[&view, &model]()->void { 
			std::vector<std::shared_ptr<PointCloud3d>> visibleClouds; 
			for each (int index in view.getVisibleElementsIndicies()) {
				visibleClouds.push_back(std::make_shared<PointCloud3d>(model.getPointCloudAt(index)));
			}
			view.render(visibleClouds);
		}
	);

	//function to update Transformation matrices and repaint visible elements
	view.setOnRequestUpdateOGLWidget(
		[this, &view, &model]()->void {
		this->_updateModelProperties();
		view.updateProjectionModelView(model.getModelProperties());
		}
	);

	//function to smooth clouds
	view.setOnRequestSmoothCloud(
		[this, &view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			model.smoothVisibleCloud(index, view.getSettings().smoothFactor);
			view.addVisibleElementToList();
		}
		this->_updateModelProperties();
	}
	);

	//function to thin clouds
	view.setOnRequestThinCloud(
		[this, &view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			model.thinVisibleCloud(index, view.getSettings().thinRadius);
			view.addVisibleElementToList();
		}
		this->_updateModelProperties();
	}
	);

	//#############################
	//### setup ui mouse events ###
	//#############################

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

void Controller::_updateModelProperties() {

	//compute average of cloud centers
	Point3d sceneCenter = Point3d(0, 0, 0);
	for each (int index in this->_view.getVisibleElementsIndicies())
	{
		sceneCenter += this->_model.getPointCloudAt(index).getCenter();
	}
	this->_model.setSceneCenterTo(sceneCenter * (1.0 / this->_view.getVisibleElementsIndicies().size()));

	//compute radius of whole scene
	double sceneRadius = 0;
	for each (int index in this->_view.getVisibleElementsIndicies())
	{
		double tmpRadius = distance3d(this->_model.getPointCloudAt(index).getCenter(), sceneCenter) + this->_model.getPointCloudAt(index).getRadius();
		if (tmpRadius > sceneRadius) sceneRadius = tmpRadius;
	}

	double overviewDistance = sceneRadius / tan((3.1415 / 180.0) * (this->_model.getModelProperties()._fieldOfViewAngleInYDirection / 2));

	this->_model.setFarClippingPlaneZTo(overviewDistance + 3 * sceneRadius);

	Point3d cameraPosition = sceneCenter + Point3d(0, 0, overviewDistance);
	this->_model.setWorldPositionTo(cameraPosition);
}
