#include "OGLWidget.h"

#include <iostream>

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr)
{
}

void OGLWidget::_renderPoints(PointCloud3d& cloud) {

	{ /* Drawing Points with VertexArrays */
		glBegin(GL_POINTS);
		glColor3ub(255, 255, 255);
		cloud.toEachPointApply(
			[](Point3d* pointPtr)->void
			{
			glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z); 
			}
		);
		glEnd();
	}
}

void OGLWidget::_renderSmoothedCloud(PointCloud3d& pointCloud, double smoothRadius) {

	PointCloud3d* newCloud = pointCloud.computeSmoothedVersionWith(smoothRadius).get();
	
	glPointSize(1);
	{ /* Drawing Points with VertexArrays */
		glBegin(GL_POINTS);
		glColor3ub(255, 255, 255);
		newCloud->toEachPointApply(
			[](Point3d* pointPtr)->void
		{
			glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z);
		}
		);
		glEnd();
	}
}

void OGLWidget::_renderNearNeighbor(PointCloud3d& pointCloud, Point3d point, double radius) {
	
	glPointSize(1);
	glBegin(GL_POINTS);
	glColor3ub(0, 0, 255);
	glVertex3d(point.x, point.y, point.z);
	glEnd();
	
	std::vector<Point3d*>* query = pointCloud.query(point, radius).get();

	if (!query->empty())
		{ /* Drawing Points with VertexArrays */
		glBegin(GL_POINTS);
		glColor3ub(255, 0, 0);
		for each (Point3d* pt in *query)
			{
			glVertex3d(pt->x, pt->y, pt->z); 
			}
		glEnd();
		}
}

void OGLWidget::render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props, SettingsContainer& settings)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color
	glClearDepth(1.0f); //clear depth buffer

	if (pointClouds.empty()) return;

	_setProjektionMatrixAccordingTo(props);

	_setCameraTransformation(pointClouds, props);

	_rotateAroundAngle(pointClouds, props);

	if (settings.showQuery) {
		_renderNearNeighbor(*pointClouds.front().get(), pointClouds.front().get()->getCenter(), settings.nnRadius);
	}
	if (settings.smooth) {
		_renderSmoothedCloud(*pointClouds.front().get(), settings.smoothFactor);
		return;
	}
	_renderPoints(*pointClouds.front().get());
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

void OGLWidget::_setCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
{
	makeCurrent();
	Point3d upVector = Point3d(0, 1, 0);
	Point3d pointCloudCenter = pointClouds.front().get()->getCenter();
	Point3d cameraPosition = props._worldPosition;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x+1, cameraPosition.y, cameraPosition.z,
		pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z,
		upVector.x, upVector.y, upVector.z
	);
}

void OGLWidget::_rotateAroundAngle(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
{
	makeCurrent();
	//rotate aroud y-axis through center
	glMatrixMode(GL_MODELVIEW);
	Point3d pointCloudCenter = pointClouds.front().get()->getCenter();
	glTranslated(pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z);
	glRotated(
		props._rotationAngle,
		0, 1, 0
	);
	glTranslated(-pointCloudCenter.x, -pointCloudCenter.y, -pointCloudCenter.z);
}

