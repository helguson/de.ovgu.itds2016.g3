#pragma once
#include "Point3d.h"
#include "PointCloud3d.h"
#include "ModelProperties.h"
#include "SettingsContainer.h"
#include <functional>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMainWindow>
//#include <gl\GL.h>
//#include <gl\GLU.h>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OGLWidget(QWidget* parentPtr = nullptr);
	void setOnRequestPaintGL(std::function<void()> callback);

	void render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props, SettingsContainer& settings);
	void render(double r, double g, double b);

private:
/*	void _setProjektionMatrixAccordingTo(ModelProperties& props);
	void _setCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props);
	void _rotateAroundAngle(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props);
	void _renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds);
*/	void _triggerOnRequestPaintGL();
	void _test();

	std::function<void()> _onRequestPaintGL;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
};
