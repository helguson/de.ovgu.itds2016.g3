#pragma once

#include "PointCloud3d.h"
#include <memory>
#include <functional>
#include <QOpenGLShaderProgram>

typedef std::shared_ptr<PointCloud3d const> sharedPointCloudPtr;
using DrawArraysFunction = std::function<void(GLenum mode, GLint first, GLsizei cout)>;

class PointCloud3dRenderer
{
public:
	PointCloud3dRenderer(DrawArraysFunction drawArrays);
	~PointCloud3dRenderer();

	void render(/*sharedPointCloudPtr pointCloudPtr*/);

private:
	QOpenGLShaderProgram _shaderProgram;
	DrawArraysFunction _drawArrays;
	
};
