uniform float Ka; //(0.0-1.0) The ambient coefficient (Ka) is used to determine how much ambient light affects a surface. 
uniform float Ks; //(0.0-1.0) The diffuse coefficient (Kd) specifies how much of the diffuse light (incident light from the light source) affects the object.
uniform float Kd; //(0.0-1.0) The specular coefficient (Ks) specifies how bright the specular reflection is.

varying vec3 vNormal;  //interpolated normal accross the polygon
varying vec3 vView; //interpolated view vector
varying vec4 vLight; //interpolated light vector

void main(void)
{
	vec3 normalVector = normalize(vNormal);
	vec3 viewVector = -normalize(vView);
	vec3 reflectVector = reflect( vLight.xyz, normalVector ); 


	//// Compute vector from surface to light position 
	//VP = vec3(gl_LightSource[i].position) - ecPosition3; 

	//// Compute distance between surface and light position 
	//d = length(VP); 

	//vec3 Att = 1.0 / (gl_LightSource[i].constantAttenuation +              //attenuation coefficient
	//				 gl_LightSource[i].linearAttenuation * d + 
	//				 gl_LightSource[i].quadraticAttenuation * d * d); 

	vec4 intensityambient  = vec4(Ka) * gl_FrontMaterial.ambient  * gl_LightSource[0].ambient;
	vec4 intensitydiffuse  = vec4(Kd) * gl_FrontMaterial.diffuse  * gl_LightSource[0].diffuse  * clamp( dot( normalVector, vLight.xyz ), 0.0, 1.0 );
	vec4 intensityspecular = vec4(Ks) * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(  clamp( dot( normalVector, vLight.xyz ), 0.0, 1.0 ), gl_FrontMaterial.shininess );
	gl_FragColor = intensityambient + intensitydiffuse + intensityspecular;
//	gl_FragColor = vec4(reflectVector, 1.0);
//	gl_FragColor = vec4(viewVector, 1.0);
//	gl_FragColor = vec4(pow( clamp( dot( normalVector, vLight.xyz ), 0.0, 1.0 ), gl_FrontMaterial.shininess ), 0.0, 0.0, 1.0);
//	gl_FragColor = vec4(vNormal.xyz, 1.0);
//	gl_FragColor = vec4(1.0);
//  gl_FragColor =  intensitydiffuse;


//  //vVertexColor = gl_Color.rgb;
//
//	//Phong lighting:
//	float diffuse = dot(lightDir.xyz, vNormal);
//	float specular = pow(clamp(dot(reflect(-(lightDir.xyz), vNormal), normalize(vViewVec.xyz)),0.0, 1.0), 16.0);
//	vec3  light = Kd * diffuse * vVertexColor + Ks * specular;
//
//	//We'll use the distance to decide how much blur we want
//	float dist = length( vViewVec );
//
//	//Fragment color with alpha value stored in.	
//	gl_FragColor = vec4( light.rgb, dist );
	}
