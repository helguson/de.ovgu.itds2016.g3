#include "QtView.h"

QtView::QtView(int numberOfArguments, char** arguments)
	:_qApplication(numberOfArguments, arguments),
	_window()
{}

int QtView::startApplication() {
	this->_window.show();
	return this->_qApplication.exec();
}

void QtView::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_window.setOnRequestPaintGL(callback);
}

void QtView::setOnRequestLoadFile(std::function<void(std::string)> callback)
{
	this->_window.setOnRequestLoadFile(callback);
}

void QtView::setOnRequestThinCloud(std::function<void()> callback)
{
	this->_window.setOnRequestThinCloud(callback);
}

void QtView::setOnRequestSmoothCloud(std::function<void()> callback)
{
	this->_window.setOnRequestSmoothCloud(callback);
}

void QtView::setOnRequestComputeNormals(std::function<void()> callback)
{
	this->_window.setOnRequestComputeNormals(callback);
}

void QtView::setOnRequestBFLine(std::function<void(int)> callback)
{
	this->_window.setOnRequestBFLine(callback);
}

void QtView::setOnRequestBFPlane(std::function<void(int)> callback)
{
	this->_window.setOnRequestBFPlane(callback);
}

void QtView::setOnRequestBFSphere(std::function<void(int)> callback)
{
	this->_window.setOnRequestBFSphere(callback);
}

void QtView::setOnRequestUpdateOGLWidget(std::function<void()> callback)
{
	this->_window.setOnRequestUpdateOGLWidget(callback);
}

void QtView::setOnRequestScroll(std::function<void(double , double )> callback)
{
	this->_window.setOnRequestScroll(callback);
}

void QtView::setOnRequestResizeWindow(std::function<void(double, double)> callback)
{
	this->_window.setOnRequestResizeWindow(callback);
}

void QtView::setOnRequestRotate(std::function<void(double, double, double, double, int, int)> callback)
{
	this->_window.setOnRequestRotate(callback);
}

void QtView::setOnRequestTranslate(std::function<void(double, double, double, double, int, int)> callback)
{
	this->_window.setOnRequestTranslate(callback);
}

void QtView::addVisibleElementToList()
{
	this->_window.addVisibleElementToList();
}

void QtView::render(std::vector<std::shared_ptr<RenderableObjects>>& objects, QMatrix4x4 modelViewProjectionMatrix) {
	this->_window.render(objects, modelViewProjectionMatrix);
}

void QtView::renderColouring(std::shared_ptr<PointCloud3d>& pointCloud, std::shared_ptr<BestFitPlane>& plane, QMatrix4x4 modelViewProjectionMatrix)
{
	this->_window.renderColouring(pointCloud, plane, modelViewProjectionMatrix);
}

void QtView::renderShading(std::shared_ptr<PointCloud3d>& pointCloud, QMatrix4x4 modelViewProjectionMatrix, QMatrix4x4 modelViewMatrix)
{
	this->_window.renderShading(pointCloud, modelViewProjectionMatrix, modelViewMatrix);
}
