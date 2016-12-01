#pragma once
#include "Point3d.h"
#include "PointCloud3d.h"
#include "ModelProperties.h"
#include "SettingsContainer.h"
#include <functional>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMainWindow>
#include <gl\GL.h>
#include <gl\GLU.h>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OGLWidget(QWidget* parentPtr = nullptr);
	void setOnRequestPaintGL(std::function<void()> callback);

	void render(PointCloud3d& pointCloud, ModelProperties& props, SettingsContainer& settings);
	void render(int r, int g, int b);

private:
	void _setProjektionMatrixAccordingTo(ModelProperties& props);
	void _setCameraTransformation(PointCloud3d& pointCloud, ModelProperties& props);
	void _rotateAroundAngle(PointCloud3d& pointCloud, ModelProperties& props);
	void _renderPoints(PointCloud3d& cloud);
	void _renderSmoothedCloud(PointCloud3d& pointCloud, double smoothFactor);
	void _renderNearNeighbor(PointCloud3d& pointCloud, Point3d point, double radius);
	void _triggerOnRequestPaintGL();

	std::function<void()> _onRequestPaintGL;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
};
