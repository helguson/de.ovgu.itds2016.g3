attribute highp vec3 vertexXYZ;
attribute lowp vec3 vertexRGB;
attribute highp vec3 worldSpaceNormal;
uniform highp mat4 modelViewProjectionMatrix;
uniform highp mat3 modelViewMatrix;
uniform highp float rasterizedSizeOfPoints;
uniform bool shouldApplyShading;
uniform highp mat3 normalMatrix;

// out to fragment shader
varying lowp vec3 fragmentRGB;
varying highp vec3 fragmentXYZInViewSpace;
varying highp vec3 viewSpaceNormal;
void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(vertexXYZ, 1.0);
	gl_PointSize = rasterizedSizeOfPoints;	// set rasterized point size

	fragmentRGB = vertexRGB;

	if(shouldApplyShading){
		// normal matrix, see http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
		fragmentXYZInViewSpace = modelViewMatrix*vertexXYZ;
		viewSpaceNormal = normalMatrix*worldSpaceNormal;
	}
}