//
uniform sampler2D toonTexture;
varying vec3 normal;

void main()
{
	vec4 color;
	float intensity;
	vec3 light = normalize(vec3(gl_LightSource[0].position));
	
	intensity = max( dot(light, normalize(normal)), 0.0);
	
	color = texture2D( toonTexture, vec2(intensity, 0.0) );
	
	/*if( intensity <= 0.125 )
	{
		color = vec4(0.2, 0.2, 0.2, 1.0);
	}
	else if( intensity > 0.125 && intensity < 0.4375)
	{
		color = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}*/
	
	//gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
	gl_FragColor = color;
}