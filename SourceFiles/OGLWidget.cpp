#include "OGLWidget.h"
#include "BestFitLine.h"

#include <iostream>

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr),
	_pointCloud3dRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitLineRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitPlaneRendererPtr()	// initialisation takes place in initializeGL
{
}


//void OGLWidget::_renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds) {
//
//	for each(std::shared_ptr<PointCloud3d> cloud in  pointClouds) {
//		{ /* Drawing Points with VertexArrays */
//			glBegin(GL_POINTS);
//			glColor3ub(255, 255, 255);
//			cloud->toEachPointApply(
//				[](Point3d* pointPtr)->void
//				{
//					glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z);
//				}
//			);
//			glEnd();
//		}
//	}
//}

void OGLWidget::render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props, SettingsContainer& settings)
{
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
		BestFitPlane plane(*pointCloud);
		this->_bestFitPlaneRendererPtr->render(
			plane,
			projectionViewModel
		);
	}

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
	this->_bestFitPlaneRendererPtr = std::make_unique<BestFitPlaneRenderer>(
		drawArrays
	);
	
	// initial settings
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f);	// set fill colour for framebuffer for glClear
	glClearDepth(1.0f);	// set fill value for depth buffer for glClear

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	// "Depth test passes if the incoming depth value is less than the stored depth value."

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
}

void OGLWidget::_triggerOnRequestPaintGL() {
	if (this->_onRequestPaintGL != nullptr ) {
		this->_onRequestPaintGL();
	}
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

//void OGLWidget::_rotateAroundAngle(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
//{
//	makeCurrent();
//	//rotate aroud y-axis through center
//	glMatrixMode(GL_MODELVIEW);
//
//	Point3d pointCloudCenter = Point3d(0, 0, 0);
//	for each(std::shared_ptr<PointCloud3d> cloud in pointClouds)
//		pointCloudCenter += cloud->getCenter();
//
//	glTranslated(pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z);
//	glRotated(
//		props._rotationAngle,
//		0, 1, 0
//	);
//	glTranslated(-pointCloudCenter.x, -pointCloudCenter.y, -pointCloudCenter.z);
//}
//
