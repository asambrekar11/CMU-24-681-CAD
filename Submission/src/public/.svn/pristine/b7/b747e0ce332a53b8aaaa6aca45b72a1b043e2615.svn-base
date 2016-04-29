// Lighting
//	Input variables: vec4 color
//	                 vec3 nomLocal
//	                 vec3 vecToCamera <- must be normalized
//	Output variable: vec4 accumColor

 HIGHP  vec3 accumColor;
accumColor=ambientColor*color.xyz;

for(int lightNo=0; lightNo<YSGLSL_MAX_NUM_LIGHT; lightNo++)
{
	if(true==lightEnabled[lightNo])
	{
		 MIDP  float diffuseIntensity=max(dot(nomLocal,lightPos[lightNo].xyz),0.0);
		accumColor+=diffuseIntensity*(color.xyz*lightColor[lightNo]);

		if(0.01<specularColor.r || 0.01<specularColor.g || 0.01<specularColor.b)
		{
			 HIGHP  vec3 unitVecToCamera=normalize(vecToCamera);
			 HIGHP  vec3 mid=normalize(lightPos[lightNo].xyz+unitVecToCamera);
			 HIGHP  float specularIntensity=pow(max(dot(mid,nomLocal),0.0),specularExponent);
			accumColor+=specularIntensity*specularColor;
		}
	}
}
