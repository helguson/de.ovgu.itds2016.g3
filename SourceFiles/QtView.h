#pragma once

#include <QApplication>
#include "MainWindow.h"

class QtView
{
public:
	QtView(int numberOfArguments, char** arguments);

	int startApplication();

	void setOnRequestPaintGL(std::function<void()> callback);
	void setOnRequestLoadFile(std::function<void(std::string)> callback);
	void setOnRequestThinCloud(std::function<void() > callback);
	void setOnRequestSmoothCloud(std::function<void() > callback);
	void setOnRequestUpdateOGLWidget(std::function<void() > callback);
	void setOnRequestScroll(std::function<void(double, double) > callback);
	void setOnRequestRotate(std::function<void(double, double, double, double, int, int) > callback);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& objects);
	void render(double r, double g, double b);
	void addVisibleElementToList();
	void updateProjectionModelView(ModelProperties& props);
	void updateRotation(Point3d axis, Point3d center, double angle);
	void updateScrolling(ModelProperties & props);
	void render(std::vector<std::shared_ptr< PointCloud3d>>& cloud, ModelProperties& props);
	SettingsContainer getSettings() { return _window.getSettings(); };
	std::vector<int> getVisibleElementsIndicies() { return _window.getVisibleElementsIndices(); };


private:
	QApplication _qApplication;
	MainWindow _window;
};
