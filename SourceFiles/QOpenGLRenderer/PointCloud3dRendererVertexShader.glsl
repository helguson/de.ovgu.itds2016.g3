attribute highp vec3 vertexXYZ;
attribute lowp vec3 vertexRGB;
uniform highp mat4 modelViewProjectionMatrix;
uniform highp float rasterizedSizeOfPoints;
varying lowp vec3 fragmentRGB;
void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(vertexXYZ, 1.0);
	//fragmentRGB = vertexRGB;
	fragmentRGB = vec3(1.0, 1.0, 1.0);

	// rasterizer settings
	gl_PointSize = rasterizedSizeOfPoints;
}