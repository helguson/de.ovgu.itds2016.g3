#include "Controller.h"

Controller::Controller(int numberOfArguments, char** arguments)
	:
	_view(numberOfArguments, arguments),
	_model()
{
	//Initialize Model and View
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
	view.addVisibleElementToList();
	});
	
	//function for rendering loop
	view.setOnRequestPaintGL( 
		[&view, &model]()->void { 
			std::vector<std::shared_ptr<RenderableObjects>> visibleObjects; 
			for each (int index in view.getVisibleElementsIndicies()) {
				visibleObjects.push_back(model.getRenderableObjectAt(index));
			}
			if (view.getSettings().showDistanceToPlane && visibleObjects.size() == 2)
			{
				std::shared_ptr<PointCloud3d> pcPtr1 = std::dynamic_pointer_cast<PointCloud3d>(visibleObjects.at(0));
				std::shared_ptr<PointCloud3d> pcPtr2 = std::dynamic_pointer_cast<PointCloud3d>(visibleObjects.at(1));
				std::shared_ptr<BestFitPlane> bfpPtr1 = std::dynamic_pointer_cast<BestFitPlane>(visibleObjects.at(0));
				std::shared_ptr<BestFitPlane> bfpPtr2 = std::dynamic_pointer_cast<BestFitPlane>(visibleObjects.at(1));

				if (pcPtr1) {
					if(bfpPtr2) view.renderColouring(pcPtr1, bfpPtr2, model.getModelViewProjectionMatrix());
					else view.render(visibleObjects, model.getModelViewProjectionMatrix());
				}

				else if (pcPtr2) {
					if (bfpPtr1) view.renderColouring(pcPtr2, bfpPtr1, model.getModelViewProjectionMatrix());
					else view.render(visibleObjects, model.getModelViewProjectionMatrix());
				}
				else 
					view.render(visibleObjects, model.getModelViewProjectionMatrix());
			}
			else if (view.getSettings().showShading && visibleObjects.size() == 1)
			{
				std::shared_ptr<PointCloud3d> pcPtr = std::dynamic_pointer_cast<PointCloud3d>(visibleObjects.at(0));

				view.renderShading(pcPtr, model.getModelViewProjectionMatrix(), model.getModelViewMatrix());
			}
			else view.render(visibleObjects, model.getModelViewProjectionMatrix());
		}
	);

	//function to update Transformation matrices and repaint visible elements
	view.setOnRequestUpdateOGLWidget(
		[this, &view, &model]()->void {
		this->_updateModelProperties();
		}
	);

	//function to smooth clouds
	view.setOnRequestSmoothCloud(
		[&view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			if(model.smoothVisibleCloud(index, view.getSettings().smoothFactor));
				view.addVisibleElementToList();
		}
	}
	);

	//function to thin clouds
	view.setOnRequestThinCloud(
		[&view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			if(model.thinVisibleCloud(index, view.getSettings().thinRadius));
				view.addVisibleElementToList();
		}
	}
	);

	view.setOnRequestComputeNormals(
		[&view, &model]()->void {
		for each (int index in view.getVisibleElementsIndicies()) {
			std::shared_ptr<PointCloud3d> pcPtr = std::dynamic_pointer_cast<PointCloud3d>(model.getRenderableObjectAt(index));
			if (pcPtr)
				model.addNormals(pcPtr, view.getSettings().thinRadius);
		}
	}
	);

	//function to create best fit line
	view.setOnRequestBFLine(
		[&view, &model](int selectedIndex)->void {
		if(model.bfLine(selectedIndex))
			view.addVisibleElementToList();
	}
	);
	//function to create best fit plane
	view.setOnRequestBFPlane(
		[&view, &model](int selectedIndex)->void {
		if (model.bfPlane(selectedIndex))
			view.addVisibleElementToList();
	}
	);
	//function create best fit sphere
	view.setOnRequestBFSphere(
		[&view, &model](int selectedIndex)->void {
		if (model.bfSphere(selectedIndex))
			view.addVisibleElementToList();
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

		double oldFieldOfViewAngle = model.getFieldOfViewAngleInYDirection();
		double newFieldOfViewAngle = oldFieldOfViewAngle * factor;

		model.setFieldOfViewAngleInYDirectionTo(newFieldOfViewAngle);
		}
	);

	view.setOnRequestRotate(
		[&view, &model](double x1, double y1, double x2, double y2, int height, int width)->void 
		{
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
		Point3d axisCs = crossProduct(lastPos3d, currPos3d);
		double  angle = acos(dotProduct(lastPos3d, currPos3d));

		QMatrix4x4 T = model.getModelViewMatrix();

		//We now multiply our current rotation axis (in camera space) with the global world transform Matrix
		//and get a new rotation axis which is now in the frame of the global transform
		Point3d axisWS;
		axisWS.x = (T(0,0) * axisCs.x) + (T(1,0) * axisCs.y) + (T(2,0) * axisCs.z);
		axisWS.y = (T(0,1) * axisCs.x) + (T(1,1) * axisCs.y) + (T(2,1) * axisCs.z);
		axisWS.z = (T(0,2) * axisCs.x) + (T(1,2) * axisCs.y) + (T(2,2) * axisCs.z);

		model.rotateCamera(axisWS, angle);
		}
	);

	view.setOnRequestTranslate(
		[&view, &model](double x1, double y1, double x2, double y2, int height, int width)->void
	{

		double lastposX = x1;
		double lastposY = -y1;
		double currposX = x2;
		double currposY = -y2;

		Point3d lastPos3d(lastposX, lastposY, 0);
		Point3d currPos3d(currposX, currposY, 0);
		double screenRadius = height / 2;
		double zoomFactor = model.getFieldOfViewAngleInYDirection() / 45;

		//compute radius of whole scene
		double sceneRadius = 0;
		for each (int index in view.getVisibleElementsIndicies())
		{
			double tmpRadius = distance3d(model.getRenderableObjectAt(index)->getCenter(), model.getSceneCenter()) + model.getRenderableObjectAt(index)->getRadius();
			if (tmpRadius > sceneRadius) sceneRadius = tmpRadius;
		}
		double ratio = sceneRadius / screenRadius / zoomFactor;

		//the current mouse interaction results in this 3d direction in camera space (unit sphere) 
		Point3d axisCs = (currPos3d-lastPos3d)* ratio;

		QMatrix4x4 T = model.getModelViewMatrix();

		//We now multiply our current direction (in camera space) with the global world transform Matrix
		//and get a new direction which is now in the frame of the global transform
		Point3d axisWS;
		axisWS.x = (T(0, 0) * axisCs.x) + (T(1, 0) * axisCs.y) + (T(2, 0) * axisCs.z);
		axisWS.y = (T(0, 1) * axisCs.x) + (T(1, 1) * axisCs.y) + (T(2, 1) * axisCs.z);
		axisWS.z = (T(0, 2) * axisCs.x) + (T(1, 2) * axisCs.y) + (T(2, 2) * axisCs.z);

		model.translateCamera(axisWS);
	}
	);

	view.setOnRequestResizeWindow(
		[&view, &model](double width, double height)->void {
		model.setWindowSize(width, height);
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
	int pointCloudCounter = 0;
	for each (int index in this->_view.getVisibleElementsIndicies())
	{
		if (std::dynamic_pointer_cast<PointCloud3d>(this->_model.getRenderableObjectAt(index))) {
			sceneCenter += this->_model.getRenderableObjectAt(index)->getCenter();
			pointCloudCounter++;
		}
	}
	this->_model.setSceneCenterTo(sceneCenter * (1.0 / pointCloudCounter));

	//compute radius of whole scene
	double sceneRadius = 0;
	for each (int index in this->_view.getVisibleElementsIndicies())
	{
		double tmpRadius = distance3d(this->_model.getRenderableObjectAt(index)->getCenter(), sceneCenter) + this->_model.getRenderableObjectAt(index)->getRadius();
		if (tmpRadius > sceneRadius) sceneRadius = tmpRadius;
	}

	double overviewDistance = sceneRadius / tan((3.1415 / 180.0) * (this->_model.getFieldOfViewAngleInYDirection() / 2));

	this->_model.setFarClippingPlaneZTo(overviewDistance + 3 * sceneRadius);

	Point3d cameraPosition = sceneCenter + Point3d(0, 0, overviewDistance);
	this->_model.setCameraPositionTo(cameraPosition);
	this->_model.reinitializeCamera();
}
