#include "OGLWidget.h"

#include <iostream>

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr)
{
}

void OGLWidget::_renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds) 
{
	for each(std::shared_ptr<PointCloud3d> cloud in  pointClouds) {

		 /* Drawing Points with VertexArrays */
			glBegin(GL_POINTS);
			glPointSize(2);
			glColor3ub(cloud->getColor().red(),cloud->getColor().green(),cloud->getColor().blue()); 
			cloud->toEachPointApply(
				[](Point3d* pointPtr)->void
			{
				glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z);
			}
			);
			glEnd();
	}

}

void OGLWidget::render(std::vector<std::shared_ptr<PointCloud3d>>& visibleElements)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color

	if (visibleElements.empty()) return;
	_renderPoints(visibleElements);

}

void OGLWidget::render(int r, int g, int b)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(r, g, b, 1.0f);   //clear background color
}

void OGLWidget::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_onRequestPaintGL = callback;
}

void OGLWidget::setOnRequestScroll(std::function<void(double, double)> callback)
{
	this->_onRequestScroll = callback;
}

void OGLWidget::setOnRequestRotate(std::function<void(double, double, double, double, int, int)> callback)
{
	this->_onRequestRotate = callback;
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
}

void OGLWidget::_triggerOnRequestPaintGL() 
{
	if (this->_onRequestPaintGL != nullptr ) {
		this->_onRequestPaintGL();
	}
}

void OGLWidget::_setProjektionMatrixAccordingTo(ModelProperties& props) 
{
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
	glMatrixMode(GL_MODELVIEW);
}

void OGLWidget::_setCameraTransformation(ModelProperties& props)
{
	Point3d upVector = Point3d(0, 1, 0);
	Point3d cameraPosition = props._worldPosition;
	makeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		props._sceneCenter.x, props._sceneCenter.y, props._sceneCenter.z,
		upVector.x, upVector.y, upVector.z
	);
}

void OGLWidget::updateProjectionModelView(ModelProperties & props)
{
	this->_setProjektionMatrixAccordingTo(props);
	this->_setCameraTransformation(props);
}

void OGLWidget::updateScrolling(ModelProperties & props)
{
	this->_setProjektionMatrixAccordingTo(props);
	this->update();
}

void OGLWidget::updateRotation(Point3d axis, Point3d center, double angle)
{
	makeCurrent();
	//The current rotation now needs to be multiplied with the global world rotation/transform
	//Therefore we ask OpenGL to give ous the global scene transform (which is stored in the Modelview-Matrix)
	double M[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, M); //note that OpenGL-Matrices are column-major 

	//We now multiply our current rotation axis (in camera space) with the global world transform Matrix
	//and get a new rotation axis which is now in the frame of the global transform
	Point3d axisWS;
	axisWS.x = (M[0] * axis.x) + (M[1] * axis.y) + (M[2] * axis.z);
	axisWS.y = (M[4] * axis.x) + (M[5] * axis.y) + (M[6] * axis.z);
	axisWS.z = (M[8] * axis.x) + (M[9] * axis.y) + (M[10] * axis.z);

	//Rotation always happens in the origin (rotation center)
	//Therefore we first "move" the frame origin to our own rotation center
	glTranslated(center.x, center.y, center.z);
	//now we rotate the frame about our own origin/rotation center
	glRotated(2 * angle * 180.0 / 3.1415, axisWS.x, axisWS.y, axisWS.z);
	//and finally we "move" the frame origin back 
	glTranslated(-center.x, -center.y, -center.z);
}



/*####################
  ### Mouse Events ###
  ####################*/

void OGLWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton)
		lastMousePosition = event->pos();
}

void OGLWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (event->buttons() != Qt::LeftButton) { return; }

	double x = (*event).x();
	double y = (*event).y();

	//No action, if mouse position outside the window
	if (x<0 || x >= width()) { return; }
	if (y<0 || y >= height()) { return; }

	if (lastMousePosition == event->pos()) return;

	this->_onRequestRotate(lastMousePosition.x(), lastMousePosition.y(), x, y, this->height(), this->width());
	lastMousePosition = event->pos();
}

void OGLWidget::wheelEvent(QWheelEvent* event)
{
	this->_onRequestScroll(event->angleDelta().x(), event->angleDelta().y());
	this->update();
}
