#include "OGLWidget.h"
#include "BestFitSphere.h"
#include <iostream>

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr),
	_pointCloud3dRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitLineRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitPlaneRendererPtr(),	// initialisation takes place in initializeGL
	_bestFitSphereRendererPtr()	// initialisation takes place in initializeGL
{
}

void OGLWidget::render(std::vector<std::shared_ptr<RenderableObjects>>& visibleElements, QMatrix4x4 transformation)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color
	float rasterizedSizeOfPoints = 2;

	if (visibleElements.empty()) return;
	for (int index = 0; index < visibleElements.size(); index++) {

		// render clouds
		if (std::dynamic_pointer_cast<PointCloud3d>(visibleElements.at(index)))
		{
			std::shared_ptr<PointCloud3d> cloud = std::dynamic_pointer_cast<PointCloud3d>(visibleElements.at(index));
			this->_pointCloud3dRendererPtr->render(
				cloud,
				transformation,
				rasterizedSizeOfPoints
			);
		}

		else if (std::dynamic_pointer_cast<BestFitLine>(visibleElements.at(index))) 
		{
			std::shared_ptr<BestFitLine> line = std::dynamic_pointer_cast<BestFitLine>(visibleElements.at(index));
			this->_bestFitLineRendererPtr->render(
				*line, 
				transformation
			);
		}
		else if (std::dynamic_pointer_cast<BestFitPlane>(visibleElements.at(index))) {

			std::shared_ptr<BestFitPlane> plane = std::dynamic_pointer_cast<BestFitPlane>(visibleElements.at(index));
			this->_bestFitPlaneRendererPtr->render(
				*plane,
				transformation
			);
		}
		else if (std::dynamic_pointer_cast<BestFitSphere>(visibleElements.at(index))) {

			std::shared_ptr<BestFitSphere> sphere = std::dynamic_pointer_cast<BestFitSphere>(visibleElements.at(index));
			this->_bestFitSphereRendererPtr->render(
				*sphere,
				transformation
			);
		}
	}
}

void OGLWidget::render(std::shared_ptr<PointCloud3d>& pointCloud, std::shared_ptr<BestFitPlane>& plane, QMatrix4x4 transformation)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color
	float rasterizedSizeOfPoints = 2;

	this->_pointCloud3dRendererPtr->render(
		pointCloud, 
		plane,
		transformation,
		rasterizedSizeOfPoints
	);

	this->_bestFitPlaneRendererPtr->render(
		*plane, transformation);

} 

void OGLWidget::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_onRequestPaintGL = callback;
}

void OGLWidget::setOnRequestRotate(std::function<void(double, double, double, double, int, int)> callback)
{
	this->_onRequestRotate = callback;
}

void OGLWidget::setOnRequestTranslate(std::function<void(double, double, double, double, int , int)> callback)
{
	this->_onRequestTranslate = callback;
}

void OGLWidget::setOnRequestResizeWindow(std::function<void(double, double)> callback)
{
	this->_onRequestResizeWindow = callback;
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
	std::cout << "invoked resizeGL" << std::endl;
	_onRequestResizeWindow(w, h);
	this->update();
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
	this->_bestFitSphereRendererPtr = std::make_unique<BestFitSphereRenderer>(
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

/*####################
  ### Mouse Events ###
  ####################*/

void OGLWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton)
		lastMousePosition = event->pos();
}

void OGLWidget::mouseMoveEvent(QMouseEvent * event)
{
	double x = (*event).x();
	double y = (*event).y();

	//No action, if mouse position outside the window
	if (x<0 || x >= width()) { return; }
	if (y<0 || y >= height()) { return; }

	if (lastMousePosition == event->pos()) return;

	if (event->buttons() == Qt::LeftButton)
		this->_onRequestRotate(lastMousePosition.x(), lastMousePosition.y(), x, y, this->height(), this->width());
	else if (event->buttons() == Qt::RightButton)
		this->_onRequestTranslate(lastMousePosition.x(), lastMousePosition.y(), x, y, this->height(), this->width());

	this->update();
	lastMousePosition = event->pos();
}

void OGLWidget::wheelEvent(QWheelEvent* event)
{
	this->_onRequestScroll(event->angleDelta().x(), event->angleDelta().y());
	this->update();
}
