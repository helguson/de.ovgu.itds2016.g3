#include "PointCloud3dRenderer.h"



PointCloud3dRenderer::PointCloud3dRenderer()
	:_shaderProgram()
{
	this->_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
		"attribute highp vec3 vextexXYZ;\n"
		"attribute lowp vec3 vertexRGB;\n"
		"uniform highp mat4 modelViewProjectionMatrix;\n"
		"varying lowp vec3 fragmentRGB;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = modelViewProjectionMatrix*vec4(vextexXYZ, 1.0);\n"
		"   fragmentRGB = vertexRGB;\n"
		"}");

	this->_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
		"varying lowp vec3 fragmentRGB;\n"
		"void main()\n"
		"{\n"
		"   gl_FragColor = vec4(fragmentRGB, 1.0);\n"
		"}");

	this->_shaderProgram.link();
		
}

PointCloud3dRenderer::~PointCloud3dRenderer()
{}

void PointCloud3dRenderer::render(/*sharedPointCloudPtr pointCloudPtr*/) {

	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.attributeLocation("modelViewProjectionMatrix");

	// access data
	const GLfloat vertexXYZs[] = {
		-1.0f, +0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
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

	// draw using current shader program
	glDrawArrays(GL_POINTS, 0, 3);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
}