#pragma once

#include "../BestFitPlane.h"
#include <QOpenGLShaderProgram>

template <typename T>
using SharedVectorPtr = std::shared_ptr<std::vector<T>>;

using DrawArraysFunction = std::function<void(GLenum mode, GLint first, GLsizei cout)>;

class BestFitPlaneRenderer
{
public:
	BestFitPlaneRenderer(DrawArraysFunction drawArrays);
	~BestFitPlaneRenderer();

	void render(
		BestFitPlane const & plane,
		QMatrix4x4 modelViewProjection
	);

private:
	QOpenGLShaderProgram _shaderProgram;
	DrawArraysFunction _drawArrays;

	static SharedVectorPtr<GLfloat> _createVectorOfPointComponents(BestFitPlane const & line);	// returns vector of components for all representative points
};

