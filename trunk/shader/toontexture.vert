varying vec3 vNormal;

void main()
{
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
		
	gl_Position = ftransform();
}




