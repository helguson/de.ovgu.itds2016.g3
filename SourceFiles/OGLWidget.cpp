#include "OGLWidget.h"

OGLWidget::OGLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	_onPaintRequest(nullptr)
{
}

OGLWidget::~OGLWidget()
{
}

void OGLWidget::_setupViewPortMatrix() {
	glViewport(0, 0, this->size().width(), this->size().height());
}

void OGLWidget::renderData(PointCloud3d& pointCloud, ModelProperties props, SettingsContainer settings)
{
	// clear image buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->_setupViewPortMatrix();

	this->_setProjektionMatrixAccordingTo();

	this->_setCameraTransformation();

	this->_rotateAroundAngle();

	// render points
	this->_renderPoints();

	// render Center
	glPointSize(10);
	glBegin(GL_POINTS);
	{
		glColor3d(1.0, 0.3, 0.3);
		Point3d center = this->_points.getCenter();
		glVertex3d(center.x, center.y, center.z);
	}
	glEnd();
}

void OGLWidget::setOnPaintRequest(std::function<void()> callback)
{
	this->_onPaintRequest = callback;
}

void OGLWidget::_renderPoints() {

	if (this->_settings.showQuery) {
		glColor3d(1.0, 0.3, 0.3);
		for each (Point3d qpt in this->_points.query(this->_points.getCenter(), this->_settings.nnRadius))
		{
			glBegin(GL_POINTS);
			{
				glVertex3d(qpt.x, qpt.y, qpt.z);
			}
			glEnd();
		}

	}
		glColor3d(0.0, 1.0, 0.0);
		glPointSize(2);
		for each (Point3d pt in this->_points.getPoints())
		{
			glBegin(GL_POINTS);
			{
				glVertex3d(pt.x, pt.y, pt.z);
			}
			glEnd();
		}
}

void OGLWidget::paintGL()
{
	if (_onPaintRequest != nullptr)
		_onPaintRequest();
}

void OGLWidget::resizeGL(int w ,int h)
{
}

void OGLWidget::initializeGL()
{

	// clear image buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	// enable depth test
	glEnable(GL_DEPTH_TEST);
}

void OGLWidget::_setProjektionMatrixAccordingTo() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double aspectRatio = (double)this->size().width() / (double)this->size().height();

	gluPerspective(
		this->_props._fieldOfViewAngleInYDirection,
		aspectRatio,
		this->_props._nearClippingPlaneZ,
		this->_props._farClippingPlaneZ
	);
}

void OGLWidget::_setCameraTransformation()
{
	Point3d upVector = Point3d(0, 1, 0);
	Point3d pointCloudCenter = this->_points.getCenter();
	Point3d cameraPosition = this->_props._worldPosition;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x+1, cameraPosition.y, cameraPosition.z,
		pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z,
		upVector.x, upVector.y, upVector.z
	);
}

void OGLWidget::_rotateAroundAngle()
{
	//rotate aroud y-axis through center
	glMatrixMode(GL_MODELVIEW);
	Point3d pointCloudCenter = _points.getCenter();
	glTranslated(pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z);
	glRotated(
		_props._rotationAngle,
		0, 1, 0
	);
	glTranslated(-pointCloudCenter.x, -pointCloudCenter.y, -pointCloudCenter.z);
}

