varying lowp vec3 fragmentRGB;
varying highp vec3 fragmentXYZInViewSpace;
varying highp vec3 viewSpaceNormal;
uniform bool shouldApplyShading;
void main()
{
	if(shouldApplyShading){
		lowp vec3 lightPositionInViewSpace = vec3(1.0, 1.0, 1.0);
		lowp vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);
		lowp vec4 diffuseColor = vec4(fragmentRGB, 1.0);
		lowp vec4 specularColor = vec4(0.7, 0.7, 0.7, 1.0);
		lowp float shininess = 100.0;
		highp vec3 N = normalize(viewSpaceNormal);
		highp vec3 L = normalize(lightPositionInViewSpace-fragmentXYZInViewSpace);	//direction to light
		highp vec3 E = normalize(-fragmentXYZInViewSpace);	// direction to camera
		highp vec3 R = normalize(-reflect(L, N));	// direction of reflected L

		lowp vec4 iAmbi = ambientColor;
		lowp vec4 iDiff = diffuseColor*max(abs(dot(N, L)), 0.0);
		iDiff = clamp(iDiff, 0.0, 1.0);
		lowp vec4 iSpec = specularColor*pow(max(dot(R, E), 0.0), 0.03*shininess);
		iSpec = clamp(iSpec, 0.0, 1.0);

		gl_FragColor = iAmbi + iDiff + iSpec;
	}
	else{
		gl_FragColor = vec4(fragmentRGB, 1.0);
	}
}