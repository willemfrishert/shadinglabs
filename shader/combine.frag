uniform sampler2D originalTex;
uniform sampler2D blurTex1;
uniform sampler2D blurTex2;
//uniform sampler2D blurTex3;
//uniform float mipmapBias;
uniform int combineTextures;

void main(void)
{
	vec4 originalColor = texture2D( originalTex, gl_TexCoord[0].xy );
	originalColor *= originalColor.a;

	if (combineTextures != 0)
	{
		vec4 sum = vec4(0.0);
		sum += texture2D(originalTex, gl_TexCoord[0].st);	
		sum += texture2D(blurTex1, gl_TexCoord[0].st);	
		sum += texture2D(blurTex2, gl_TexCoord[0].st);
		//texture2D(blurTex3, gl_TexCoord[0].st);		
		
		gl_FragColor = sum/3.0;
		gl_FragColor = sum;		
	}
	else
	{
		gl_FragColor = originalColor;
	}
} 