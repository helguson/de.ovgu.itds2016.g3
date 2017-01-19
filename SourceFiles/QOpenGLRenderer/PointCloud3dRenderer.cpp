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
	QMatrix4x4 modelViewProjectionMatrix,
	float rasterizedSizeOfPoints
)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");
	int rasterizedSizeOfPointsLocation = this->_shaderProgram.uniformLocation("rasterizedSizeOfPoints");
	int shoulApplyShadingLocation = this->_shaderProgram.uniformLocation("shouldApplyShading");

	// access data
	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(pointCloudPtr);
	SharedVectorPtr<GLfloat> vertexRGBs = _createVectorOfPointColourComponents(pointCloudPtr);

	size_t numberOfVertices = pointCloudPtr->getNumberOfPoints();
	size_t numberOfComponentsInXYZ = 3;
	size_t numberOfComponentsInRGB = 3;

	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInXYZ);
	this->_shaderProgram.enableAttributeArray(vertexRGBLocation);
	this->_shaderProgram.setAttributeArray(vertexRGBLocation, vertexRGBs->data(), numberOfComponentsInRGB);

	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, modelViewProjectionMatrix);
	this->_shaderProgram.setUniformValue(rasterizedSizeOfPointsLocation, rasterizedSizeOfPoints);
	this->_shaderProgram.setUniformValue(shoulApplyShadingLocation, false);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_POINTS, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
}

void PointCloud3dRenderer::renderColouring(SharedPointCloudPtr pointCloudPtr, SharedBestFitPlanePtr planePtr, QMatrix4x4 transformation, float rasterizedSizeOfPoints)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");
	int rasterizedSizeOfPointsLocation = this->_shaderProgram.uniformLocation("rasterizedSizeOfPoints");
	int shoulApplyShadingLocation = this->_shaderProgram.uniformLocation("shouldApplyShading");


	// access data
	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(pointCloudPtr);
	SharedVectorPtr<GLfloat> vertexRGBs = _createVectorOfPointColourComponents(pointCloudPtr, planePtr);

	size_t numberOfVertices = pointCloudPtr->getNumberOfPoints();
	size_t numberOfComponentsInXYZ = 3;
	size_t numberOfComponentsInRGB = 3;

	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInXYZ);
	this->_shaderProgram.enableAttributeArray(vertexRGBLocation);
	this->_shaderProgram.setAttributeArray(vertexRGBLocation, vertexRGBs->data(), numberOfComponentsInRGB);

	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, transformation);
	this->_shaderProgram.setUniformValue(rasterizedSizeOfPointsLocation, rasterizedSizeOfPoints);
	this->_shaderProgram.setUniformValue(shoulApplyShadingLocation, false);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_POINTS, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
}

void PointCloud3dRenderer::renderShading(SharedPointCloudPtr pointCloudPtr,  QMatrix4x4 modelViewPrjoectionMatrix, QMatrix4x4 modelViewMatrix, float rasterizedSizeOfPoints)
{
	this->_shaderProgram.bind();

	int vertexXYZLocation = this->_shaderProgram.attributeLocation("vertexXYZ");
	int vertexRGBLocation = this->_shaderProgram.attributeLocation("vertexRGB");
	int worldSpaceNormalLocation = this->_shaderProgram.attributeLocation("worldSpaceNormal");
	int modelViewProjectionMatrixLocation = this->_shaderProgram.uniformLocation("modelViewProjectionMatrix");
	int modelViewMatrixLocation = this->_shaderProgram.uniformLocation("modelViewMatrix");
	int NormalMatrixLocation = this->_shaderProgram.uniformLocation("normalMatrix");
	int rasterizedSizeOfPointsLocation = this->_shaderProgram.uniformLocation("rasterizedSizeOfPoints");
	int shoulApplyShadingLocation = this->_shaderProgram.uniformLocation("shouldApplyShading");


	// access data
	SharedVectorPtr<GLfloat> vertexXYZs = _createVectorOfPointComponents(pointCloudPtr);
	SharedVectorPtr<GLfloat> normalXYZs = _createVectorOfNormalPointComponents(pointCloudPtr);
	SharedVectorPtr<GLfloat> vertexRGBs = _createVectorOfPointColourComponents(pointCloudPtr);

	size_t numberOfVertices = pointCloudPtr->getNumberOfPoints();
	size_t numberOfComponentsInXYZ = 3;
	size_t numberOfComponentsInRGB = 3;

	this->_shaderProgram.enableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.setAttributeArray(vertexXYZLocation, vertexXYZs->data(), numberOfComponentsInXYZ);
	this->_shaderProgram.enableAttributeArray(worldSpaceNormalLocation);
	this->_shaderProgram.setAttributeArray(worldSpaceNormalLocation, normalXYZs->data(), numberOfComponentsInXYZ);
	this->_shaderProgram.enableAttributeArray(vertexRGBLocation);
	this->_shaderProgram.setAttributeArray(vertexRGBLocation, vertexRGBs->data(), numberOfComponentsInRGB);

	this->_shaderProgram.setUniformValue(modelViewProjectionMatrixLocation, modelViewPrjoectionMatrix);
	this->_shaderProgram.setUniformValue(modelViewMatrixLocation, modelViewMatrix.toGenericMatrix<3,3>());
	this->_shaderProgram.setUniformValue(NormalMatrixLocation, modelViewMatrix.normalMatrix());
	this->_shaderProgram.setUniformValue(rasterizedSizeOfPointsLocation, rasterizedSizeOfPoints);
	this->_shaderProgram.setUniformValue(shoulApplyShadingLocation, true);

	// ---------------------------------
	// draw using current shader program
	this->_drawArrays(GL_POINTS, 0, numberOfVertices);

	this->_shaderProgram.disableAttributeArray(vertexXYZLocation);
	this->_shaderProgram.disableAttributeArray(vertexRGBLocation);
	this->_shaderProgram.disableAttributeArray(worldSpaceNormalLocation);
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

SharedVectorPtr<GLfloat> PointCloud3dRenderer::_createVectorOfNormalPointComponents(SharedPointCloudPtr pointCloudPtr)
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();
	std::shared_ptr<std::vector<Point3d>> normals = pointCloudPtr->getNormals();
	size_t numberOfComponents = 3; // x, y, z
	result->reserve(pointCloudPtr->getNormals()->size() * numberOfComponents);

	for (int index = 0; index < pointCloudPtr->getNormals()->size(); index++) {
		result->push_back(pointCloudPtr->getNormals()->at(index).x);
		result->push_back(pointCloudPtr->getNormals()->at(index).y);
		result->push_back(pointCloudPtr->getNormals()->at(index).z); 
	}
	return result;
}

SharedVectorPtr<GLfloat> PointCloud3dRenderer::_createVectorOfPointColourComponents(SharedPointCloudPtr pointCloudPtr)
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	size_t numberOfComponents = 3; // r, g, b
	result->reserve(pointCloudPtr->getNumberOfPoints() * numberOfComponents);

	pointCloudPtr->toEachPointApply(
		[result](Point3d const * point)->void {
		result->push_back(1.0);
		result->push_back(1.0);
		result->push_back(1.0);
	}
	);

	return result;
}

SharedVectorPtr<GLfloat> PointCloud3dRenderer::_createVectorOfPointColourComponents(SharedPointCloudPtr pointCloudPtr, SharedBestFitPlanePtr planePtr)
{
	SharedVectorPtr<GLfloat> result = std::make_shared<std::vector<GLfloat>>();

	size_t numberOfComponents = 3; // r, g, b
	result->reserve(pointCloudPtr->getNumberOfPoints() * numberOfComponents);
	std::shared_ptr<double> maxDistance = std::make_shared<double>(0.0);

	pointCloudPtr->toEachPointApply(
		[maxDistance, planePtr](Point3d const * point)->void {
		if (*maxDistance < abs(planePtr->computeDistanceTo(*point))) *maxDistance = abs(planePtr->computeDistanceTo(*point));
	}
	);

	pointCloudPtr->toEachPointApply(
		[maxDistance, planePtr, result](Point3d const * point)->void {
		QColor color;
		color.setHsvF(0.75 - (abs(planePtr->computeDistanceTo(*point)) * 0.75 / *maxDistance), 1.0, 1.0);
		result->push_back(color.redF());
		result->push_back(color.greenF());
		result->push_back(color.blueF());
	}
	);

	return result;
}