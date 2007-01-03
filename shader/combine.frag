uniform sampler2D originalTexture;
uniform sampler2D blurTexture1;
uniform sampler2D blurTexture2;
uniform sampler2D blurTexture3;

void main(void)
{
	vec4 sum = vec4(0.0);
	// get 4 textures and add them together
	sum += texture2D(originalTexture, gl_TexCoord[0].st);	
	sum += texture2D(blurTexture1, gl_TexCoord[0].st);	
	sum += texture2D(blurTexture2, gl_TexCoord[0].st);
	sum += texture2D(blurTexture3, gl_TexCoord[0].st);		
	gl_FragColor = sum;		
} 