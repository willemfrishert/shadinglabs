/*!
 * blureffect.frag
 * 
 * Willem Frishert
 */

uniform float sampleDistance;
uniform sampler2D textureMap;
uniform bool horizontalBlur;

const int KNumberOfSamples = 13;

void main(void)
{
	// either horizontal blur or vertical blur
	vec2 blurType = vec2( float(horizontalBlur), float(!horizontalBlur) );
	vec4 sum = vec4 ( 0.0 );

	vec2 samples[KNumberOfSamples];
	samples[0]  = vec2( -0.6 );
	samples[1]  = vec2( -0.5 );
	samples[2]  = vec2( -0.4 );
	samples[3]  = vec2( -0.3 );
	samples[4]  = vec2( -0.2 );
	samples[5]  = vec2( -0.1 );
	samples[6]	= vec2(  0.0 ); // actual location
	samples[7]  = vec2(  0.1 );
	samples[8]  = vec2(  0.2 );
	samples[9]  = vec2(  0.3 );
	samples[10] = vec2(  0.4 );
	samples[11] = vec2(  0.5 );
	samples[12] = vec2(  0.6 );

	for (int i = 0; i < KNumberOfSamples; i++)
	{
	  sum += texture2D( textureMap, gl_TexCoord[0].st + sampleDistance * (samples[i] * blurType) );
	}

	sum /= float(KNumberOfSamples);
	gl_FragColor = vec4( sum );
}
