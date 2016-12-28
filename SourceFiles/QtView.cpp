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

void QtView::setOnRequestUpdateOGLWidget(std::function<void()> callback)
{
	this->_window.setOnRequestUpdateOGLWidget(callback);
}

void QtView::setOnRequestScroll(std::function<void(double , double )> callback)
{
	this->_window.setOnRequestScroll(callback);
}

void QtView::setOnRequestRotate(std::function<void(double, double, double, double, int, int)> callback)
{
	this->_window.setOnRequestRotate(callback);
}

void QtView::render(double r, double g, double b) {
	this->_window.render(r, g, b);
}
void QtView::addVisibleElementToList()
{
	this->_window.addVisibleElementToList();
}

void QtView::updateProjectionModelView(ModelProperties& props) {
	this->_window.updateProjectionModelView(props);
}

void QtView::updateRotation(Point3d axis, Point3d center, double angle)
{
	this->_window.updateRotation(axis, center, angle);
}

void QtView::updateScrolling(ModelProperties & props)
{
	this->_window.updateScrolling(props);
}

void QtView::render(std::vector<std::shared_ptr<RenderableObjects>>& objects, ModelProperties& props) {
	this->_window.render(objects, props);
}
