varying vec3 vNormal; //normal at vertex
varying vec3 vView; // view vector towards vertex

void main(void)
{
	//calculate the normal per vertex and have it interpolated.
	vNormal = gl_NormalMatrix * gl_Normal;

	//calculate the view vector and have it interpolated.
	vView = vec3(gl_ModelViewMatrix * gl_Vertex);
  
	//3d Screen space:
	//These two instructions are pretty much the same. ftransform is more precise though.
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
}
