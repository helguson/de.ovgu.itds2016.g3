#include "PointCloud3dRenderer.h"

#include <iostream>

PointCloud3dRenderer::PointCloud3dRenderer(DrawArraysFunction drawArrays)
	:_shaderProgram(),
	_drawArrays(drawArrays)
{
	// initialize shader program
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Vertex, 
		"SourceFiles/QOpenGLRenderer/PointCloud3dRendererVertexShader.glsl"
	);
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/QOpenGLRenderer/PointCloud3dRendererFragmentShader.glsl"
	);
	this->_shaderProgram.link();
}

PointCloud3dRenderer::~PointCloud3dRenderer()
{}


void PointCloud3dRenderer::render(
	SharedPointCloudPtr pointCloudPtr,
	QMatrix4x4 modelViewProjection,
	float rasterizedSizeOfPoints
)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
//	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");
	int rasterizedSizeOfPointsLocation = this->_shaderProgram.uniformLocation("rasterizedSizeOfPoints");

	// access data
	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(pointCloudPtr);

//	const GLfloat vertexRGBs[] = {
//		1.0f, 0.0f, 0.0f,
//		0.0f, 1.0f, 0.0f,
//		0.0f, 0.0f, 1.0f
//	};

	size_t numberOfVertices = pointCloudPtr->getNumberOfPoints();
	size_t numberOfComponentsInXYZ = 3;
//	size_t numberOfComponentsInRGB = 3;

	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInXYZ);
//	this->_shaderProgram.enableAttributeArray(vertexRGBLocation);
//	this->_shaderProgram.setAttributeArray(vertexRGBLocation, vertexRGBs, numberOfComponentsInRGB);

	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, modelViewProjection);
	this->_shaderProgram.setUniformValue(rasterizedSizeOfPointsLocation, rasterizedSizeOfPoints);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_POINTS, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
//	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
}

SharedVectorPtr<GLfloat> PointCloud3dRenderer::_createVectorOfPointComponents(SharedPointCloudPtr pointCloudPtr) 
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	size_t numberOfComponents = 3; // x, y, z
	result->reserve(pointCloudPtr->getNumberOfPoints() * numberOfComponents);

	pointCloudPtr->toEachPointApply(
		[result](Point3d const * point)->void{
			result->push_back(point->x);
			result->push_back(point->y);
			result->push_back(point->z);
		}
	);

	return result;
}