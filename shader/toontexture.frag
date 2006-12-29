uniform sampler2D lightMap;
varying vec3 normal, lightDir, viewDir;

void main()
{
	vec3 normalNormalized = normalize(normal);
	vec3 viewDirection = normalize(viewDir);

	float dotLN = clamp( dot( lightDir, normalNormalized ), 0.0, 1.0 );
	float dotVN = abs( dot( viewDirection, normalNormalized ) );

	vec4 intensity = texture2D( lightMap, vec2( dotLN, 0.0 ) );
  
  gl_FragColor = vec4( intensity.xyz, 1.0 );
}



