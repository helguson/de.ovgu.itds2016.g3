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
			std::vector<std::shared_ptr<RenderableObjects>> visibleObjects; 
			for each (int index in view.getVisibleElementsIndicies()) {
				visibleObjects.push_back(model.getRenderableObjectAt(index));
			}
			view.render(visibleObjects);
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
			if(model.smoothVisibleCloud(index, view.getSettings().smoothFactor));
				view.addVisibleElementToList();
		}
		this->_updateModelProperties();
	}
	);

	//function to thin clouds
	view.setOnRequestThinCloud(
		[this, &view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			if(model.thinVisibleCloud(index, view.getSettings().thinRadius));
				view.addVisibleElementToList();
		}
		this->_updateModelProperties();
	}
	);

	//#############################
	//### setup ui mouse events ###
	//#############################

	view.setOnRequestScroll(
		[&view, &model ](double offsetX, double offsetY)->void {

		double maxOffsetAccordingToObservations = 16;
		double dMax = 0.1;
		double factor = 1 + dMax*(offsetY / maxOffsetAccordingToObservations);

		double oldFieldOfViewAngle = model.getModelProperties()._fieldOfViewAngleInYDirection;
		double newFieldOfViewAngle = oldFieldOfViewAngle * factor;

		model.setFieldOfViewAngleInYDirectionTo(newFieldOfViewAngle);
		view.updateScrolling(model.getModelProperties());
		}
	);

	view.setOnRequestRotate(
		[&view, &model](double x1, double y1, double x2, double y2, int height, int width)->void {
		//The center of our virtual rotation ball is in the center of the screen
		const double x0 = width / 2.0;
		const double y0 = height / 2.0;
		//We set the radius of rotation ball to half of the screen height
		const double r = height / 2.0; //ball radius is half the window height
		//const double r  = sqrt(sqr(m_winWidth) + sqr(m_winHeight)) / 2; //ball radius is half the window diagonal;

		double lastposX = x1;
		double lastposY = y1;
		double currposX = x2;
		double currposY = y2;

		double lastPosZ = (sqr(r) - sqr(lastposX - x0) - sqr(lastposY - y0));
		double currPosZ = (sqr(r) - sqr(currposX - x0) - sqr(currposY - y0));

		//if z is negative then we are outside the virtual ball and the rotation is just around the current view direction/z-axis
		if (lastPosZ<0) lastPosZ = 0;
		else            lastPosZ = sqrt(lastPosZ);

		if (currPosZ<0) currPosZ = 0;
		else            currPosZ = sqrt(currPosZ);

		//take into account that the screen origin is in the top left corner (and not bottom left) -> x'=x-x0 and y'=y0-y
		Point3d lastPos3d(lastposX - x0, y0 - lastposY, lastPosZ);
		Point3d currPos3d(currposX - x0, y0 - currposY, currPosZ);

		normalizeVector(lastPos3d); //make unit normal vector
		normalizeVector(currPos3d); //make unit normal vector

		//the current mouse interaction results in this 3d rotation in camera space (unit sphere) 
		Point3d axisCS = crossProduct(lastPos3d, currPos3d);
		double  angle = acos(dotProduct(lastPos3d, currPos3d));
		double fov = model.getModelProperties()._fieldOfViewAngleInYDirection;

		model.setFieldOfViewAngleInYDirectionTo(45);
		view.updateScrolling(model.getModelProperties());
		view.updateRotation(axisCS, model.getModelProperties()._sceneCenter, angle); 
		model.setFieldOfViewAngleInYDirectionTo(fov);
		view.updateScrolling(model.getModelProperties());
	}
	);
	
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
		sceneCenter += this->_model.getRenderableObjectAt(index)->getCenter();
	}
	this->_model.setSceneCenterTo(sceneCenter * (1.0 / this->_view.getVisibleElementsIndicies().size()));

	//compute radius of whole scene
	double sceneRadius = 0;
	for each (int index in this->_view.getVisibleElementsIndicies())
	{
		double tmpRadius = distance3d(this->_model.getRenderableObjectAt(index)->getCenter(), sceneCenter) + this->_model.getRenderableObjectAt(index)->getRadius();
		if (tmpRadius > sceneRadius) sceneRadius = tmpRadius;
	}

	double overviewDistance = sceneRadius / tan((3.1415 / 180.0) * (this->_model.getModelProperties()._fieldOfViewAngleInYDirection / 2));

	this->_model.setFarClippingPlaneZTo(overviewDistance + 3 * sceneRadius);

	Point3d cameraPosition = sceneCenter + Point3d(0, 0, overviewDistance);
	this->_model.setWorldPositionTo(cameraPosition);
}
