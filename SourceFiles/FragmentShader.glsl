varying lowp vec3 fragmentRGB;
void main()
{
	gl_FragColor = vec4(fragmentRGB, 1.0);
}