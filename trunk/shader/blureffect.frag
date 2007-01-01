uniform float sampleDistance;
uniform sampler2D textureMap;
uniform float horizontalBlur; // either 0 or 1
uniform float verticalBlur;	  // either 0 or 1

void main(void)
{
	vec2 blurType = vec2( horizontalBlur, verticalBlur );
	vec4 sum = vec4 ( 0.0 );

	vec2 samples[13];
	samples[0]  = vec2( -0.6, -0.6 );
	samples[1]  = vec2( -0.5, -0.5 );
	samples[2]  = vec2( -0.4, -0.4 );
	samples[3]  = vec2( -0.3, -0.3 );
	samples[4]  = vec2( -0.2, -0.2 );
	samples[5]  = vec2( -0.1, -0.1 );
	samples[6]	= vec2(  0.0,  0.0 ); // actual location
	samples[7]  = vec2(  0.1,  0.1 );
	samples[8]  = vec2(  0.2,  0.2 );
	samples[9]  = vec2(  0.3,  0.3 );
	samples[10] = vec2(  0.4,  0.4 );
	samples[11] = vec2(  0.5,  0.5 );
	samples[12] = vec2(  0.6,  0.6 );

	for (int i = 0; i < 13; i++)
	{
	  sum += texture2D( textureMap, gl_TexCoord[0].st + sampleDist * (samples[i] * blurType) );
	}

	sum /= 13.0;
	gl_FragColor = vec4( sum );
}
