#include "OGLWidget.h"
#include "BestFitLine.h"

#include <iostream>
#include "PointCloud3dRenderer.h"

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr),
	_pointCloud3dRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitLineRendererPtr()	// initialisation takes place in initializeGL
{
}

void OGLWidget::_renderPointCloud(std::shared_ptr<PointCloud3d>& cloud) 
{
<<<<<<< HEAD
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

void OGLWidget::render(std::vector<std::shared_ptr<RenderableObjects>>& visibleElements)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color

	if (visibleElements.empty()) return;
	for (int index = 0; index < visibleElements.size(); index++) {

		// render clouds
		if (std::dynamic_pointer_cast<PointCloud3d>(visibleElements.at(index)))
		{
			std::shared_ptr<PointCloud3d> cloud = std::dynamic_pointer_cast<PointCloud3d>(visibleElements.at(index));
			_renderPointCloud(cloud);
		}

		//TODO: render anything else


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//_setProjektionMatrixAccordingTo(props);
	//_setCameraTransformation(pointClouds, props);
	//_rotateAroundAngle(pointClouds, props);

	QMatrix4x4 model;
	QMatrix4x4 view = this->_getCameraTransformation(pointClouds, props);
	QMatrix4x4 projection = this->_getProjektionMatrixAccordingTo(props);


	QMatrix4x4 projectionViewModel = projection*view*model;
	float rasterizedSizeOfPoints = 2;

	for (std::shared_ptr<PointCloud3d> pointCloud : pointClouds) {

		this->_pointCloud3dRendererPtr->render(
			pointCloud,
			projectionViewModel,
			rasterizedSizeOfPoints
		);

		//TODO: manage best fit object in model
		BestFitLine line(*pointCloud);
		this->_bestFitLineRendererPtr->render(
			line,
			projectionViewModel
		);
	}
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
	std::cout << "invoked resizeGL" << std::endl;
	glViewport(0, 0, w,h);
}

void OGLWidget::initializeGL()
{
	std::cout << "invoked initializeGL" << std::endl;
	this->initializeOpenGLFunctions();

	DrawArraysFunction drawArrays = [this](GLenum mode, GLint first, GLsizei count)->void
	{
		this->glDrawArrays(mode, first, count);
	};

	this->_pointCloud3dRendererPtr = std::make_unique<PointCloud3dRenderer>(
		drawArrays
	);
	this->_bestFitLineRendererPtr = std::make_unique<BestFitLineRenderer>(
		drawArrays
	);
	
	// initial settings
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f);	// set fill colour for framebuffer for glClear
	glClearDepth(1.0f);	// set fill value for depth buffer for glClear

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	// "Depth test passes if the incoming depth value is less than the stored depth value."

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
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
QMatrix4x4 OGLWidget::_getProjektionMatrixAccordingTo(ModelProperties& props) {

	QMatrix4x4 result;

	double aspectRatio = (double)this->size().width() / (double)this->size().height();

	result.perspective(
		props._fieldOfViewAngleInYDirection,
		aspectRatio,
		props._nearClippingPlaneZ,
		props._farClippingPlaneZ
	);
	
	return result;
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


QMatrix4x4 OGLWidget::_getCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
{
	QMatrix4x4 result;

	std::function<QVector3D(Point3d const &)> toQVector3D = [](Point3d const & point)->QVector3D {
		return QVector3D(point.x, point.y, point.z);
	};

	Point3d upVector(0, 1, 0);

	Point3d pointCloudCenter(0, 0, 0);
	for each(std::shared_ptr<PointCloud3d> cloud in pointClouds)
		pointCloudCenter += cloud->getCenter();

	Point3d cameraPosition = props._worldPosition;

	result.lookAt(
		toQVector3D(cameraPosition),
		toQVector3D(pointCloudCenter),
		toQVector3D(upVector)
	);

	return result;
}
