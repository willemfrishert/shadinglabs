uniform sampler2D textureMap;
uniform float thresholdBrightness;

void main(void)
{
	vec4 texel = texture2D( textureMap, gl_TexCoord[0].st );
	
	// calculate the new intensity using a sigmoidal function (smoothstep)
	vec3 newIntensity = smoothstep( vec3( thresholdBrightness ), vec3(1.0), texel.rgb );

	gl_FragColor = vec4( newIntensity , texel.a );

}
