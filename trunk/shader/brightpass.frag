uniform sampler2D textureMap;
uniform float thresholdBrightness;

vec3 RGBtoHSL( vec3 aRGBColor )
	{
	float H = 0.0;
	float L = 0.0;
	float S = 0.0;
	
	float R = aRGBColor.r;
	float G = aRGBColor.g;
	float B = aRGBColor.b;
	
	float minimumValue = min ( min( R, G ), B );	//Min. value of RGB
	float maximumValue = max ( max( R, G ), B );	//Max. value of RGB
	float delta = maximumValue - minimumValue;									//Delta RGB value
 
	L = ( maximumValue + minimumValue ) / 2.0;

	if ( delta == 0.0 )          //This is a gray, no chroma...
		{
			H = 0.0;     //HSL results = 0 / 1
			S = 0.0;
		}
	else                       //Chromatic data...
		{
		if ( L < 0.5 )
			{
			S = delta / ( maximumValue + minimumValue );
			}
		else  
			{
			S = delta / ( 2.0 - maximumValue - minimumValue );
			}
			
		float deltaRed = ( ( ( maximumValue - R ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
		float deltaGreen = ( ( ( maximumValue - G ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
		float deltaBlue = ( ( ( maximumValue - B ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

		if ( R == maximumValue )
			{
			H = deltaBlue - deltaGreen;
			}
		else 
			{
			if ( G == maximumValue )
				{
				H = ( 1.0 / 3.0 ) + deltaRed - deltaBlue;
				}
			else
				{
				if ( B == maximumValue )
					{
					H = ( 2.0 / 3.0 ) + deltaGreen - deltaRed;
					}
				}
			}
			
		if ( H < 0.0 )
			{
			H += 1.0;
			}
			
		if ( H > 1.0 );
			{
			H -= 1.0;
			}
		}
		
	return vec3( H, S, L );
	}

float Hue_2_RGB( float v1, float v2, float vH )             //Function Hue_2_RGB
{
   if ( vH < 0.0 ) vH += 1.0;
   if ( vH > 1.0 ) vH -= 1.0;
   if ( ( 6.0 * vH ) < 1.0 ) return ( v1 + ( v2 - v1 ) * 6.0 * vH );
   if ( ( 2.0 * vH ) < 1.0 ) return ( v2 );
   if ( ( 3.0 * vH ) < 2.0 ) return ( v1 + ( v2 - v1 ) * ( ( 2.0 / 3.0 ) - vH ) * 6.0 );
   return ( v1 );
}


vec3 HSLtoRGB( vec3 HSLColor )
	{
	float R = 0.0;
	float G = 0.0;
	float B = 0.0;
	
	float H = HSLColor.x;
	float S = HSLColor.y;
	float L = HSLColor.z;
		
	if ( S == 0.0 )                       //HSL values = 0 Ö 1
		{
		R = L;                     //RGB results = 0 Ö 255
		G = L;
		B = L;
	}
	else
		{
		float var_1;
		float var_2;
		
		if ( L < 0.5 )
			{
			var_2 = L * ( 1.0 + S );
			}
		else
			{
			var_2 = ( L + S ) - ( S * L );
			}
		
	   var_1 = 2.0 * L - var_2;

	   R = Hue_2_RGB( var_1, var_2, H + ( 1.0 / 3.0 ) );
	   G = Hue_2_RGB( var_1, var_2, H );
	   B = Hue_2_RGB( var_1, var_2, H - ( 1.0 / 3.0 ) );
	   }
	   
	return vec3( R, G, B );
}


void main(void)
	{
	vec4 texel = texture2D( textureMap, gl_TexCoord[0].st );
	
	// deterimining the minimum and maximum value of the texel
	float minimumValue = min ( min( texel.r, texel.g ), texel.b );	
	float maximumValue = max ( max( texel.r, texel.g ), texel.b );
	
	// Calculate L of HSL
	float texelLightness = ( maximumValue + minimumValue ) / 2.0;
	
	// multiplier = 0.2 when texelLightness < 0.5, else multiplier = 1.2;
	float multiplier =  step( 0.6, texelLightness ) + 0.1;
	  
	// multiplier = 0.0 when texel.rstv < 0.5, else multiplier = 1.0;
//	vec4 stepResult = step( vec4(0.5), texel ) * 2.0;
//	float multiplier = stepResult.s*stepResult.t*stepResult.r;

//	gl_FragColor =  texel*multiplier;
	
//	gl_FragColor = pow( texel, vec4(15.0) ) * 7.0;

	gl_FragColor = vec4( smoothstep( vec3( thresholdBrightness ), vec3(1.0), texel.rgb ) , 1.0);
	
	
//	vec3 HSL = RGBtoHSL( vec3(texel.rgb) );
//	vec3 RGB = HSLtoRGB( HSL );
	
//	gl_FragColor = vec4( HSL.rgb, texel.a);

//	gl_FragColor = vec4( pow(vec3(texel.rgb), vec3(10.0)), texel.a)*2.0;
	}
