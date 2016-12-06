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
	void render(std::vector<std::shared_ptr<PointCloud3d>>& cloud);
	void render(double r, double g, double b);
	void addVisibleElementToList();
	void updateProjectionModelView(ModelProperties& props);
	SettingsContainer getSettings() { return _window.getSettings(); };
	std::vector<int> getVisibleElementsIndicies() { return _window.getVisibleElementsIndices(); };

private:
	QApplication _qApplication;
	MainWindow _window;
};