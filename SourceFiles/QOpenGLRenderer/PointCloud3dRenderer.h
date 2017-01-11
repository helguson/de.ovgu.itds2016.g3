#pragma once

#include "../PointCloud3d.h"
#include <memory>
#include <functional>
#include <QOpenGLShaderProgram>
#include <vector>

using SharedPointCloudPtr = std::shared_ptr<PointCloud3d const>;

template <typename T>
using SharedVectorPtr = std::shared_ptr<std::vector<T>>;

using DrawArraysFunction = std::function<void(GLenum mode, GLint first, GLsizei cout)>;

class PointCloud3dRenderer
{
public:
	PointCloud3dRenderer(DrawArraysFunction drawArrays);
	~PointCloud3dRenderer();

	void render(
		SharedPointCloudPtr pointCloudPtr,
		QMatrix4x4 transformation,
		float rasterizedSizeOfPoints
	);

private:
	QOpenGLShaderProgram _shaderProgram;
	DrawArraysFunction _drawArrays;
	static SharedVectorPtr<GLfloat> _createVectorOfPointComponents(SharedPointCloudPtr pointCloudPtr);	// returns vector of components for all represented point (x1, y1, z1, ... xn, yn, zn)
	static SharedVectorPtr<GLfloat> _createVectorOfPointColourComponents(SharedPointCloudPtr pointCloudPtr); // returns vector of colour components for all represented points (r1, g1, b1, ... rn, gn, bn)
};
