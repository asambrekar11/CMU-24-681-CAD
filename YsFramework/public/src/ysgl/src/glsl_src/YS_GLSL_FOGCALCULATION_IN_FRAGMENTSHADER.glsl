if(false!=fogEnabled)
{
	 MIDP  float d=fogDensity*abs(zInViewCoord);
	 MIDP  float f=clamp(exp(-d*d),0.0,1.0);
	 LOWP  vec3 fogMix=mix(fogColor.rgb,gl_FragColor.rgb,f);
	gl_FragColor[0]=fogMix[0];
	gl_FragColor[1]=fogMix[1];
	gl_FragColor[2]=fogMix[2];
}
