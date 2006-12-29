varying vec3 normal, lightDir, viewDir;

void main()
{	
	lightDir = normalize(vec3(gl_LightSource[0].position));
	normal = normalize(gl_NormalMatrix * gl_Normal);
	viewDir = -normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
		
	gl_Position = ftransform();
}




