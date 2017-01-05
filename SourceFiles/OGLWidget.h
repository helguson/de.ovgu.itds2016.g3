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
#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMainWindow>

#include "QOpenGLRenderer/PointCloud3dRenderer.h"
#include "QOpenGLRenderer/BestFitLineRenderer.h"

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OGLWidget(QWidget* parentPtr = nullptr);
	void setOnRequestPaintGL(std::function<void()> callback);
	void setOnRequestScroll(std::function<void(double, double)> callback);
	void setOnRequestRotate(std::function<void(double, double, double, double, int, int) > callback);
	void updateProjectionModelView(ModelProperties& props);
	void updateRotation(Point3d axis, Point3d center, double angle);
	void updateScrolling(ModelProperties& props);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& pointClouds);
	void render(int r, int g, int b);

private:
	QMatrix4x4 _getProjektionMatrixAccordingTo(ModelProperties& props);
	QMatrix4x4 _getCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props);
	void _renderPointCloud(std::shared_ptr<PointCloud3d>& pointClouds);
	void _triggerOnRequestPaintGL();
	void _setProjektionMatrixAccordingTo(ModelProperties& props);
	void _setCameraTransformation(ModelProperties& props);
	QPoint lastMousePosition;
	std::function<void()> _onRequestPaintGL;
	std::function<void(double, double)> _onRequestScroll;
	std::function<void(double, double, double, double, int, int) > _onRequestRotate;
	std::unique_ptr<PointCloud3dRenderer> _pointCloud3dRendererPtr;
	std::unique_ptr<BestFitLineRenderer> _bestFitLineRendererPtr;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event) override;
};
