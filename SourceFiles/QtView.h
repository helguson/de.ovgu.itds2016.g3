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
	void setOnRequestBFLine(std::function<void(int) > callback);
	void setOnRequestBFPlane(std::function<void(int) > callback);
	void setOnRequestBFSphere(std::function<void(int) > callback);
	void setOnRequestUpdateOGLWidget(std::function<void() > callback);
	void setOnRequestScroll(std::function<void(double, double) > callback);
	void setOnRequestResizeWindow(std::function<void(double, double) > callback);
	void setOnRequestRotate(std::function<void(double, double, double, double, int, int) > callback);
	void setOnRequestTranslate(std::function<void(double, double, double, double, int ,int)> callback);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& objects, QMatrix4x4 transformation);
	void renderColouring(std::shared_ptr<PointCloud3d>& pointCloud, std::shared_ptr<BestFitPlane>& plane, QMatrix4x4 transformation);
	void renderShading(std::shared_ptr<PointCloud3d>& pointcloud, QMatrix4x4 transformation);
	void addVisibleElementToList();
	SettingsContainer getSettings() { return _window.getSettings(); };
	std::vector<int> getVisibleElementsIndicies() { return _window.getVisibleElementsIndices(); };


private:
	QApplication _qApplication;
	MainWindow _window;
};
