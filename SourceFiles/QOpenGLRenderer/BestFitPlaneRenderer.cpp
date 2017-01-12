#include "BestFitPlaneRenderer.h"



BestFitPlaneRenderer::BestFitPlaneRenderer(DrawArraysFunction drawArrays)
	:_shaderProgram(),
	_drawArrays(drawArrays)
{
	// initialize shader program
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Vertex,
		"SourceFiles/QOpenGLRenderer/BestFitPlaneRendererVertexShader.glsl"
	);
	this->_shaderProgram.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/QOpenGLRenderer/BestFitPlaneRendererFragmentShader.glsl"
	);
	this->_shaderProgram.link();
}

BestFitPlaneRenderer::~BestFitPlaneRenderer()
{}

void BestFitPlaneRenderer::render(
	BestFitPlane const & plane,
	QMatrix4x4 modelViewProjection
)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");

	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(plane);

	size_t numberOfVertices = plane.getRepresentativePoints().size();
	size_t numberOfComponentsInVertexXYZ = 3;


	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInVertexXYZ);
	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, modelViewProjection);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_LINE_LOOP, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
}

SharedVectorPtr<GLfloat> BestFitPlaneRenderer::_createVectorOfPointComponents(BestFitPlane const & plane) 
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	for (Point3d const & point : plane.getRepresentativePoints()) {
		result->push_back(point.x);
		result->push_back(point.y);
		result->push_back(point.z);
	}

	return result;
}
