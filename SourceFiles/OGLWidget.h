#pragma once
#include "Point3d.h"
#include "PointCloud3d.h"
#include "ModelProperties.h"
#include "SettingsContainer.h"
#include <functional>
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

	void render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props, SettingsContainer& settings);

private:
	QMatrix4x4 _getProjektionMatrixAccordingTo(ModelProperties& props);
	QMatrix4x4 _getCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props);
/*	void _rotateAroundAngle(std::vector<std::shared_ptr<PointCloud3d>>& pointCloud, ModelProperties& props);
	void _renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds);
*/	void _triggerOnRequestPaintGL();

	std::function<void()> _onRequestPaintGL;
	std::unique_ptr<PointCloud3dRenderer> _pointCloud3dRendererPtr;
	std::unique_ptr<BestFitLineRenderer> _bestFitLineRendererPtr;
	std::unique_ptr<BestFitPlaneRenderer> _bestFitPlaneRendererPtr;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
};
