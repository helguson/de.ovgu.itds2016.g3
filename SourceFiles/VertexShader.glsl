attribute highp vec3 vertexXYZ;
attribute lowp vec3 vertexRGB;
uniform highp mat4 modelViewProjectionMatrix;
varying lowp vec3 fragmentRGB;
void main()
{
	//gl_Position = vec4(0.0, 0.0, 0.0, 1.0); // origin of kartesian volume
	gl_Position = vec4(vertexXYZ, 1.0);
	//gl_Position = modelViewProjectionMatrix*vec4(vertexXYZ, 1.0);
	fragmentRGB = vertexRGB;
}