#pragma once
#include "Point3d.h"
#include "PointCloud3d.h"
#include "ModelProperties.h"
#include "SettingsContainer.h"
#include <functional>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>

class OGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OGLWidget(QWidget *parent);
	~OGLWidget();
	void renderData(PointCloud3d& pointCloud, ModelProperties props, SettingsContainer settings);
	void setOnPaintRequest(std::function<void()> callback);

private:
	PointCloud3d _points;
	ModelProperties _props;
	SettingsContainer _settings;
	void _setupViewPortMatrix();
	void _setProjektionMatrixAccordingTo();
	void _setCameraTransformation();
	void _rotateAroundAngle();
	void _renderPoints();
	std::function<void()> _onPaintRequest; 

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int , int);
};
