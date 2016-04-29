if(YSGLSL_TEX_TYPE_NONE==textureType)
{
}
else if(YSGLSL_TEX_TYPE_TILING==textureType ||
        YSGLSL_TEX_TYPE_BILLBOARD==textureType ||
        YSGLSL_TEX_TYPE_ATTRIBUTE==textureType)
{
	 LOWP  vec4 texcell=texture2D(textureIdent,texCoordOut.xy);
	gl_FragColor=gl_FragColor*texcell;
}
else if(YSGLSL_TEX_TYPE_3DTILING==textureType)
{
	 LOWP  vec4 texcell[3],avg;
	texcell[0]=texture2D(textureIdent,texCoordOut.xy);
	texcell[1]=texture2D(textureIdent,texCoordOut.xz);
	texcell[2]=texture2D(textureIdent,texCoordOut.yz);
	avg=texcell[0]/3.0+texcell[1]/3.0+texcell[2]/3.0;
	gl_FragColor=gl_FragColor*avg;
}

