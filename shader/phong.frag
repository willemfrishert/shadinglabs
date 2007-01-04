/*!
 * phong.frag
 * 
 * Willem Frishert
 */


uniform float Ka; //(0.0-1.0)
uniform float Ks; //(0.0-1.0)
uniform float Kd; //(0.0-1.0)

varying vec3 vNormal;  //interpolated normal accross the polygon
varying vec3 vView; //interpolated view vector

void main(void)
{
	vec3 normalVector  = normalize(vNormal);
	vec3 viewVector    = normalize(vView);
	vec3 reflectVector = reflect( gl_LightSource[0].position.xyz, normalVector ); 

	vec4 intensityAmbient  = vec4(Ka) * gl_FrontMaterial.ambient  * gl_LightSource[0].ambient;
	vec4 intensityDiffuse  = vec4(Kd) * gl_FrontMaterial.diffuse  * gl_LightSource[0].diffuse  * clamp( dot( normalVector, gl_LightSource[0].position.xyz ), 0.0, 1.0 );
	vec4 intensitySpecular = vec4(Ks) * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow( clamp( dot( reflectVector, viewVector ), 0.0, 1.0 ), gl_FrontMaterial.shininess );
	gl_FragColor = intensityAmbient + intensityDiffuse + intensitySpecular;
}
