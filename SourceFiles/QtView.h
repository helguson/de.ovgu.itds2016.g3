#pragma once

#include <QApplication>
#include <qmatrix4x4.h>
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
	void setOnRequestResizeWindow(std::function<void(double, double) > callback);
	void setOnRequestRotate(std::function<void(double, double, double, double, int, int) > callback);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& objects, QMatrix4x4 transformation);
	void addVisibleElementToList();
	void repaint();
	SettingsContainer getSettings() { return _window.getSettings(); };
	std::vector<int> getVisibleElementsIndicies() { return _window.getVisibleElementsIndices(); };


private:
	QApplication _qApplication;
	MainWindow _window;
};
