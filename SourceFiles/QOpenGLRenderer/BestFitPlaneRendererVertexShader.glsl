attribute highp vec3 vertexXYZ;
uniform highp mat4 modelViewProjectionMatrix;
void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(vertexXYZ, 1.0);
}