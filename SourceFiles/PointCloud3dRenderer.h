#pragma once

#include "PointCloud3d.h"
#include <memory>
#include <QOpenGLShaderProgram>

typedef std::shared_ptr<PointCloud3d const> sharedPointCloudPtr;

class PointCloud3dRenderer
{
public:
	PointCloud3dRenderer();
	~PointCloud3dRenderer();

	void render(/*sharedPointCloudPtr pointCloudPtr*/);

private:
	QOpenGLShaderProgram _shaderProgram;
	
};
