varying vec3 vNormal;
varying vec3 vView;
varying vec4 vLight;

void main(void)
{
	//calculate the normal per vertex and have it interpolated.
	vNormal = gl_NormalMatrix * gl_Normal;

	//calculate the view vector and have it interpolated.
	vView = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	//calualte the light vector and have it interpolated.
	vLight = gl_LightSource[0].position;
  
	//3d Screen space:
	// (These two are the same)
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}
