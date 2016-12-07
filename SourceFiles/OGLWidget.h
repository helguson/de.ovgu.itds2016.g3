#pragma once
#include "Point3d.h"
#include "PointCloud3d.h"
#include "ModelProperties.h"
#include "SettingsContainer.h"
#include <functional>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMainWindow>
#include <QWheelEvent>
#include <gl\GL.h>
#include <gl\GLU.h>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OGLWidget(QWidget* parentPtr = nullptr);
	void setOnRequestPaintGL(std::function<void()> callback);
	void setOnRequestScroll(std::function<void(double, double)> callback);

	void render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds);
	void updateProjectionModelView(ModelProperties& props);
	
	void render(int r, int g, int b);

private:

	void _renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds);
	void _triggerOnRequestPaintGL();
	void _setProjektionMatrixAccordingTo(ModelProperties& props);
	void _setCameraTransformation(ModelProperties& props);
	//void _rotateAroundAngle(ModelProperties& props);

	std::function<void()> _onRequestPaintGL;
	std::function<void(double, double)> _onRequestScroll;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	//void mousePressEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
	//void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event) override;
};
