#include "OGLWidget.h"

#include <iostream>

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr)
{
}

void OGLWidget::_renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds) {

	glBegin(GL_POINTS);
	for each(std::shared_ptr<PointCloud3d> cloud in  pointClouds) {
		{ /* Drawing Points with VertexArrays */
			
			glColor3ub(255, 255, 255);
			cloud->toEachPointApply(
				[](Point3d* pointPtr)->void
			{
				glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z);
			}
			);
			
		}
	}
	glEnd();
}

void OGLWidget::render(std::vector<std::shared_ptr<PointCloud3d>>& visibleElements)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color
	glClearDepth(1.0f); //clear depth buffer

	if (visibleElements.empty()) return;
	_renderPoints(visibleElements);
}

void OGLWidget::updateProjectionModelView(ModelProperties & props)
{
	this->_setProjektionMatrixAccordingTo(props);
	this->_setCameraTransformation(props);
}

void OGLWidget::render(int r, int g, int b)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(r, g, b, 1.0f);   //clear background color
	glClearDepth(1.0f);                     //clear depth buffer
}

void OGLWidget::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_onRequestPaintGL = callback;
}

void OGLWidget::setOnRequestScroll(std::function<void(double, double)> callback)
{
	this->_onRequestScroll = callback;
}

void OGLWidget::paintGL()
{
	std::cout << "invoked paintGL" << std::endl;
	this->_triggerOnRequestPaintGL();
}

void OGLWidget::resizeGL(int w ,int h)
{
	glViewport(0, 0, w,h);
}

void OGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	
	// clear image buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
}

void OGLWidget::_triggerOnRequestPaintGL() {
	if (this->_onRequestPaintGL != nullptr ) {
		this->_onRequestPaintGL();
	}
}

void OGLWidget::_setProjektionMatrixAccordingTo(ModelProperties& props) {

	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double aspectRatio = (double)this->size().width() / (double)this->size().height();

	gluPerspective(
		props._fieldOfViewAngleInYDirection,
		aspectRatio,
		props._nearClippingPlaneZ,
		props._farClippingPlaneZ
	);
}

void OGLWidget::_setCameraTransformation(ModelProperties& props)
{
	
	Point3d upVector = Point3d(0, 1, 0);
	Point3d cameraPosition = props._worldPosition;
	makeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x+1, cameraPosition.y, cameraPosition.z,
		props._sceneCenter.x, props._sceneCenter.y, props._sceneCenter.z,
		upVector.x, upVector.y, upVector.z
	);
}

/*####################
  ### Mouse Events ###
  ####################*/

void OGLWidget::wheelEvent(QWheelEvent* event)
{
	this->_onRequestScroll(event->angleDelta().x(), event->angleDelta().y());
	this->update();
}
