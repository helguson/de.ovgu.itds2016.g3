#pragma once
#include "BestFitLine.h"
#include "Point3d.h"
#include <functional>
#include <vector> 
#include <QOpenGLShaderProgram>

template <typename T>
using SharedVectorPtr = std::shared_ptr<std::vector<T>>;

using DrawArraysFunction = std::function<void(GLenum mode, GLint first, GLsizei cout)>;

class BestFitLineRenderer
{
public:
	BestFitLineRenderer(DrawArraysFunction drawArrays);
	~BestFitLineRenderer();

	void render(BestFitLine const & line, QMatrix4x4 transformation);

private:
	QOpenGLShaderProgram _shaderProgram;
	DrawArraysFunction _drawArrays;

	static SharedVectorPtr<GLfloat> _createVectorOfPointComponents(BestFitLine const & line);	// returns vector of components for all representative points
};

