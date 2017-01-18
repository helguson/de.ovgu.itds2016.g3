#include "BestFitSphereRenderer.h"

BestFitSphereRenderer::BestFitSphereRenderer(DrawArraysFunction drawArrays)
	:_shaderProgram(),
	_drawArrays(drawArrays)
{
	// initialize shader program
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Vertex,
		"SourceFiles/QOpenGLRenderer/BestFitSphereRendererVertexShader.glsl"
	);
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/QOpenGLRenderer/BestFitSphereRendererFragmentShader.glsl"
	);
	this->_shaderProgram.link();
}

BestFitSphereRenderer::~BestFitSphereRenderer()
{}

void BestFitSphereRenderer::render(BestFitSphere const & sphere, QMatrix4x4 transformation)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");

	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(sphere);

	size_t numberOfVertices = sphere.getRepresentativePoints().size();
	size_t numberOfComponentsInVertexXYZ = 3;


	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInVertexXYZ);
	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, transformation);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_LINE_STRIP, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
}

SharedVectorPtr<GLfloat> BestFitSphereRenderer::_createVectorOfPointComponents(BestFitSphere const & sphere) 
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	for (Point3d const & point : sphere.getRepresentativePoints()) {
		result->push_back(point.x);
		result->push_back(point.y);
		result->push_back(point.z);
	}

	return result;
}
