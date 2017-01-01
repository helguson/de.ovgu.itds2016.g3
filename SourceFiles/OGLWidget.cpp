#include "OGLWidget.h"

#include <iostream>
#include "PointCloud3dRenderer.h"

OGLWidget::OGLWidget(QWidget *parentPtr)
	:
	QOpenGLWidget(parentPtr),
	_onRequestPaintGL(nullptr)
{
}


//void OGLWidget::_renderPoints(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds) {
//
//	for each(std::shared_ptr<PointCloud3d> cloud in  pointClouds) {
//		{ /* Drawing Points with VertexArrays */
//			glBegin(GL_POINTS);
//			glColor3ub(255, 255, 255);
//			cloud->toEachPointApply(
//				[](Point3d* pointPtr)->void
//				{
//					glVertex3d(pointPtr->x, pointPtr->y, pointPtr->z);
//				}
//			);
//			glEnd();
//		}
//	}
//}

void OGLWidget::render(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props, SettingsContainer& settings)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //clear background color
	//glClearDepth(1.0f); //clear depth buffer
	//
	//if (pointClouds.empty()) return;

	//_setProjektionMatrixAccordingTo(props);

	//_setCameraTransformation(pointClouds, props);

	//_rotateAroundAngle(pointClouds, props);

	//_renderPoints(pointClouds);
}

void OGLWidget::render(double r, double g, double b)
{

	//glClearColor(r, g, b, 0.0f);   //clear background color
	//glClearDepth(1.0f);                     //clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffers

	/*DrawArraysFunction drawArraysFunction = [this](GLenum mode, GLint first, GLsizei count)->void
	{
		this->glDrawArrays(mode, first, count);
	};
	PointCloud3dRenderer renderer(drawArraysFunction);
	renderer.render();*/

	this->_test();
}

void OGLWidget::_test()
{
	// create shader program
	QOpenGLShaderProgram program;
	program.addShaderFromSourceFile(
		QOpenGLShader::Vertex,
		"SourceFiles/VertexShader.glsl"
	);
	program.addShaderFromSourceFile(
		QOpenGLShader::Fragment,
		"SourceFiles/FragmentShader.glsl"
	);
	program.link();
	program.bind();
	int vertexXYZLocation = program.attributeLocation("vertexXYZ");
	int vertexRGBLocation = program.attributeLocation("vertexRGB");
	int modelViewProjectionMatrixLocation = program.attributeLocation("modelViewProjectionMatrix");

	// bind data
	const GLfloat vertexXYZs[] = {
		-0.5f, -0.5f, +0.0f,
		+0.5f, -0.5f, +0.0f,
		+0.0f, +0.5f, +0.0f
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

	program.enableAttributeArray(vertexXYZLocation);
	program.setAttributeArray(vertexXYZLocation, vertexXYZs, numberOfComponentsInXYZ);
	program.enableAttributeArray(vertexRGBLocation);
	program.setAttributeArray(vertexRGBLocation, vertexRGBs, numberOfComponentsInRGB);

	program.setUniformValue(modelViewProjectionMatrixLocation, modelViewProjectionMatrix);

	// draw arrays
	this->glDrawArrays(GL_TRIANGLES, 0, 3);	// draw lines using 3 vertices as well as additional attributes.
}


void OGLWidget::setOnRequestPaintGL(std::function<void()> callback)
{
	this->_onRequestPaintGL = callback;
}

void OGLWidget::paintGL()
{
	std::cout << "invoked paintGL" << std::endl;
	this->_triggerOnRequestPaintGL();
}

void OGLWidget::resizeGL(int w ,int h)
{
	std::cout << "invoked resizeGL" << std::endl;
	glViewport(0, 0, w,h);
}

void OGLWidget::initializeGL()
{
	std::cout << "invoked initializeGL" << std::endl;
	this->initializeOpenGLFunctions();
	
	// initial settings
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f);	// set fill colour for framebuffer for glClear
	glClearDepth(1.0f);	// set fill value for depth buffer for glClear

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	// "Depth test passes if the incoming depth value is less than the stored depth value."

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear image buffer
}

void OGLWidget::_triggerOnRequestPaintGL() {
	if (this->_onRequestPaintGL != nullptr ) {
		this->_onRequestPaintGL();
	}
}

//void OGLWidget::_setProjektionMatrixAccordingTo(ModelProperties& props) {
//
//	makeCurrent();
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	double aspectRatio = (double)this->size().width() / (double)this->size().height();
//
//	gluPerspective(
//		props._fieldOfViewAngleInYDirection,
//		aspectRatio,
//		props._nearClippingPlaneZ,
//		props._farClippingPlaneZ
//	);
//}
//
//void OGLWidget::_setCameraTransformation(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
//{
//	makeCurrent();
//	Point3d upVector = Point3d(0, 1, 0);
//
//	Point3d pointCloudCenter = Point3d(0, 0, 0);
//	for each(std::shared_ptr<PointCloud3d> cloud in pointClouds)
//		pointCloudCenter += cloud->getCenter();
//
//	Point3d cameraPosition = props._worldPosition;
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(
//		cameraPosition.x+1, cameraPosition.y, cameraPosition.z,
//		pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z,
//		upVector.x, upVector.y, upVector.z
//	);
//}
//
//void OGLWidget::_rotateAroundAngle(std::vector<std::shared_ptr<PointCloud3d>>& pointClouds, ModelProperties& props)
//{
//	makeCurrent();
//	//rotate aroud y-axis through center
//	glMatrixMode(GL_MODELVIEW);
//
//	Point3d pointCloudCenter = Point3d(0, 0, 0);
//	for each(std::shared_ptr<PointCloud3d> cloud in pointClouds)
//		pointCloudCenter += cloud->getCenter();
//
//	glTranslated(pointCloudCenter.x, pointCloudCenter.y, pointCloudCenter.z);
//	glRotated(
//		props._rotationAngle,
//		0, 1, 0
//	);
//	glTranslated(-pointCloudCenter.x, -pointCloudCenter.y, -pointCloudCenter.z);
//}
//
