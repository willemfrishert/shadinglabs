/*!
 * toontexture.frag
 * 
 * Willem Frishert
 */

uniform sampler2D lightMap;
uniform float red, green, blue;
varying vec3 vNormal;

void main()
{
	vec3 vNormalized = normalize(vNormal);

	// calculate angle between light and surface normal
	float dotLN = clamp( dot( gl_LightSource[0].position.xyz, vNormalized ), 0.0, 1.0 );

	// use dot-product as index for lookup table
	vec4 intensity = texture2D( lightMap, vec2( dotLN, 0.0 ) );	

	gl_FragColor = vec4( intensity.r * red, intensity.g * green, intensity.b * blue, 1.0 );
}



