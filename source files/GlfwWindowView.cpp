#include "GlfwWindowView.h"
#include <iostream>

GlfwWindowView::GlfwWindowView()
	:
	_isGlfwInitialized(false),
	_isWindowInitialized(false),
	_windowPtr(nullptr),
	_callbackAdapter()
{

	this->_initializeLibrary();
	this->_initializeGlfwWindow();
	this->_initializeCallbackAdapter();
}

GlfwWindowView::~GlfwWindowView() {

	glfwDestroyWindow(this->_windowPtr);
	glfwTerminate();
}

bool GlfwWindowView::isCorrectlyInitialized() {

	return this->_isGlfwInitialized && this->_isWindowInitialized;
}

void GlfwWindowView::_initializeLibrary() {

	this->_isGlfwInitialized = glfwInit() == GLFW_TRUE;
}

void GlfwWindowView::_initializeGlfwWindow() {

	this->_windowPtr = glfwCreateWindow(
		640,
		480,
		"hello world",
		NULL,
		NULL
	);

	this->_isWindowInitialized = this->_windowPtr != nullptr;
}

void GlfwWindowView::_initializeCallbackAdapter() {

	if (this->_isWindowInitialized) {
		this->_callbackAdapter.hookInto(this->_windowPtr);
	}
}

void GlfwWindowView::render(PointCloud3d pointCloud, CameraModel cameraModel, ProjectionModel projectionModel, double rotationAngleAroundYAxis) {

	if (this->isCorrectlyInitialized()) {
		this->_initializeImage();
		this->_renderImage(pointCloud, cameraModel, projectionModel, rotationAngleAroundYAxis);
		this->_showImage();
		this->_pollInteractionsWithWindow();

	}
	else {
		// TODO: log warning
	}
}

void GlfwWindowView::_initializeImage() {

	// Make the window's context current
	glfwMakeContextCurrent(this->_windowPtr);

	// clear image buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
}

void GlfwWindowView::_setupViewportMatrix() {

	glViewport(0, 0, this->_getWidth(), this->_getHeight());
}

void renderAsArray(PointCloud3d pointCloud) {

	if (!pointCloud.getPoints().empty())
	{
		glPointSize(2);
		glColor3d(1, 1, 1);

		/* Drawing Points with VertexArrays */
		glEnableClientState(GL_VERTEX_ARRAY); //enable data upload to GPU

		unsigned int numberOfCoordinates = 3;
		unsigned int strideBetweenStartOfElementData = sizeof(Point3d);
		void* dataPtr = &pointCloud.getPoints()[0];
		unsigned int initialElementIndex = 0;
		unsigned int numberOfElements = (unsigned int)pointCloud.getPoints().size();

		glVertexPointer(numberOfCoordinates, GL_DOUBLE, strideBetweenStartOfElementData, dataPtr);
		// draw point cloud
		glDrawArrays(GL_POINTS, initialElementIndex, numberOfElements);

		glDisableClientState(GL_VERTEX_ARRAY);  //disable data upload to GPU
	}
}

void renderIndividual(PointCloud3d& pointCloud) {

	glPointSize(1);
	if (!pointCloud.getPoints().empty())
	{ /* Drawing Points with VertexArrays */
		glBegin(GL_POINTS);
		glColor3ub(255, 255, 255);
		for each (Point3d pt in pointCloud.getPoints())
		{
			glVertex3d(pt.x, pt.y, pt.z);
		}
		glEnd();
	}
}

void renderNearNeighbor(PointCloud3d& pointCloud, Point3d point, double radius) {

	glBegin(GL_POINTS);
	glPointSize(1);
	glColor3ub(0, 0, 255);
	glVertex3d(point.x, point.y, point.z);
	glEnd();

	glPointSize(1);
	std::vector<Point3d> queryResult = pointCloud.query(point, radius);
	if (!queryResult.empty())
	{ /* Drawing Points with VertexArrays */
		glBegin(GL_POINTS);
		glColor3ub(255, 0, 0);
		for each (Point3d pt in queryResult)
		{
			glVertex3d(pt.x, pt.y, pt.z);
		}
		glEnd();
	}
	
	
}

void renderCenterOf(PointCloud3d pointCloud) {

	glPointSize(10);
	glBegin(GL_POINTS);
	{
		glColor3d(1.0, 0.3, 0.3);
		Point3d center = pointCloud.getCenter();
		glVertex3d(center.x, center.y, center.z);
	}
	glEnd();
}

void GlfwWindowView::_renderImage(PointCloud3d pointCloud, CameraModel cameraModel, ProjectionModel projectionModel, double rotationAngleAroundYAxis) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer

	this->_setupViewportMatrix();

	this->_setProjektionMatrixAccordingTo(projectionModel);

	this->_setCameraTransformation(pointCloud.getCenter(), cameraModel);

	//rotate aroud y-axis through center
	glMatrixMode(GL_MODELVIEW);
	Point3d pointCloudCenter = pointCloud.getCenter();
	glTranslated(pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z);
	glRotated(
		rotationAngleAroundYAxis,
		0, 1, 0
	);
	glTranslated(-pointCloudCenter.x, -pointCloudCenter.y, -pointCloudCenter.z);

	// render points
	renderNearNeighbor(pointCloud, pointCloud.getPoints()[0], 5);
	renderIndividual(pointCloud);

	renderCenterOf(pointCloud);

}

void GlfwWindowView::_showImage() {

	// Swap front and back buffers
	glfwSwapBuffers(this->_windowPtr);
}

void GlfwWindowView::_pollInteractionsWithWindow() {

	// Poll for and process events
	glfwPollEvents();
}

void GlfwWindowView::setOnScrollCallbackTo(std::function<void(double, double)> callback) {

	this->_callbackAdapter.setOnMouseWheelCallbackTo(
		[callback](GLFWwindow* windowPtr, double offsetX, double offsetY)->void { callback(offsetX, offsetY); }
	);
}

void GlfwWindowView::setOnDemandClosingOfWindowCallbackTo(std::function<void()> callback) {

	this->_callbackAdapter.setOnDemandClosingWindowCallbackTo(
		[callback](GLFWwindow* windowPtr)->void { callback();  }
	);
}

void GlfwWindowView::setOnKeyCallbackTo(std::function<void(GLFWwindow*, int, int, int, int)> callback) {

	this->_callbackAdapter.setOnKeyCallbackTo(callback);
}

void GlfwWindowView::_setProjektionMatrixAccordingTo(ProjectionModel projectionModel) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double aspectRatio = (double)this->_getWidth() / (double)this->_getHeight();

	gluPerspective(
		projectionModel.getFieldOfViewAngleInYDirection(),
		aspectRatio,
		projectionModel.getNearClippingPlaneZ(),
		projectionModel.getFarClippingPlaneZ()
	);
}

void GlfwWindowView::_setCameraTransformation(Point3d pointCloudCenter, CameraModel cameraModel)
{
	Point3d upVector = Point3d(0, 1, 0);

	Point3d cameraPosition = cameraModel.getWorldPosition();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z, 
		pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z,
		upVector.x, upVector.y, upVector.z
	);
}

int GlfwWindowView::_getWidth() {
	int width, height;
	glfwGetFramebufferSize(this->_windowPtr,&width,&height);
	return width;
}

int GlfwWindowView::_getHeight() {
	int width, height;
	glfwGetFramebufferSize(this->_windowPtr, &width, &height);
	return height;
}
