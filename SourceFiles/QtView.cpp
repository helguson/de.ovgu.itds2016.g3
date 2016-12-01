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

void QtView::render(double r, double g, double b) {
	this->_window.render(r, g, b);
}

void QtView::render(PointCloud3d& cloud, ModelProperties& props) {
	this->_window.render(cloud, props);
}