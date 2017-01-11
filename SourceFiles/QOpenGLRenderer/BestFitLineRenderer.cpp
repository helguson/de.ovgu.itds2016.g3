#include "BestFitLineRenderer.h"

BestFitLineRenderer::BestFitLineRenderer(DrawArraysFunction drawArrays)
	:_shaderProgram(),
	_drawArrays(drawArrays)
{
	// initialize shader program
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Vertex,
		"SourceFiles/QOpenGLRenderer/BestFitLineRendererVertexShader.glsl"
	);
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/QOpenGLRenderer/BestFitLineRendererFragmentShader.glsl"
	);
	this->_shaderProgram.link();
}

BestFitLineRenderer::~BestFitLineRenderer()
{}

void BestFitLineRenderer::render(BestFitLine const & line, QMatrix4x4 transformation)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");

	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(line);

	size_t numberOfVertices = line.getRepresentativePoints().size();
	size_t numberOfComponentsInVertexXYZ = 3;


	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInVertexXYZ);
	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, transformation);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_LINE_STRIP, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
}

SharedVectorPtr<GLfloat> BestFitLineRenderer::_createVectorOfPointComponents(BestFitLine const & line) 
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	for (Point3d const & point : line.getRepresentativePoints()) {
		result->push_back(point.x);
		result->push_back(point.y);
		result->push_back(point.z);
	}

	return result;
}
