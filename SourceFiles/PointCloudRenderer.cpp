#include "PointCloud3dRenderer.h"

#include <iostream>

PointCloud3dRenderer::PointCloud3dRenderer(DrawArraysFunction drawArrays)
	:_shaderProgram()
{
	this->_drawArrays = drawArrays;

	// initialize shader program
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Vertex, 
		"SourceFiles/VertexShader.glsl"
	);
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/FragmentShader.glsl"
	);
	this->_shaderProgram.link();
}

PointCloud3dRenderer::~PointCloud3dRenderer()
{}

void PointCloud3dRenderer::render(/*sharedPointCloudPtr pointCloudPtr*/) {

	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");
	int rasterizedSizeOfPointsLocation = this->_shaderProgram.uniformLocation("rasterizedSizeOfPoints");

	// access data
	const GLfloat vertexXYZs[] = {
		-1.0f, -1.0f, +0.0f,
		+1.0f, -1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f
	};
	const GLfloat vertexRGBs[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	size_t numberOfVertices = 3;
	size_t numberOfComponentsInXYZ = 3;
	size_t numberOfComponentsInRGB = 3;

	QMatrix4x4 modelViewProjectionMatrix;
	modelViewProjectionMatrix.setToIdentity();	// identity matrix

	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs, numberOfComponentsInXYZ);
	this->_shaderProgram.enableAttributeArray(vertexRGBLocation);
	this->_shaderProgram.setAttributeArray(vertexRGBLocation, vertexRGBs, numberOfComponentsInRGB);

	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, modelViewProjectionMatrix);

	float rasterizedSizeOfPoints = 30.f;
	this->_shaderProgram.setUniformValue(rasterizedSizeOfPointsLocation, rasterizedSizeOfPoints);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_POINTS, 0, 3);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
}