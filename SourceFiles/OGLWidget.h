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
#include "QOpenGLRenderer/BestFitPlaneRenderer.h"

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	OGLWidget(QWidget* parentPtr = nullptr);
	void setOnRequestPaintGL(std::function<void()> callback);
	void setOnRequestScroll(std::function<void(double, double)> callback);
	void setOnRequestRotate(std::function<void(double, double, double, double, int, int) > callback);
	void setOnRequestTranslate(std::function<void(double, double, double, double, int, int) > callback);
	void setOnRequestResizeWindow(std::function<void(double, double)> callback);
	void render(std::vector<std::shared_ptr<RenderableObjects>>& pointClouds, QMatrix4x4 transformation);
	void render(std::shared_ptr<PointCloud3d>& pointCloud,std::shared_ptr<BestFitPlane>& plane, QMatrix4x4 transformation);

private:
	void _triggerOnRequestPaintGL();
	QPoint lastMousePosition;
	std::function<void()> _onRequestPaintGL;
	std::function<void(double, double)> _onRequestScroll;
	std::function<void(double, double)> _onRequestResizeWindow;
	std::function<void(double, double, double, double, int, int) > _onRequestRotate;
	std::function<void(double, double, double, double, int, int) > _onRequestTranslate;
	std::unique_ptr<PointCloud3dRenderer> _pointCloud3dRendererPtr;
	std::unique_ptr<BestFitLineRenderer> _bestFitLineRendererPtr;
	std::unique_ptr<BestFitPlaneRenderer> _bestFitPlaneRendererPtr;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void mousePressEvent(QMouseEvent *event);
	//void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event) override;
};
