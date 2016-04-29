void Ys_ProcessPointSprite()
{
	if(pointSizeMode==YSGLSL_POINTSPRITE_SIZE_IN_PIXEL)
	{
		gl_PointSize=pointSizeIn;
	}
	else if(pointSizeMode==YSGLSL_POINTSPRITE_SIZE_IN_3DSPACE)
	{
		HIGHP vec4 prj1=projection*modelView*vec4(vertex,1.0);
		HIGHP vec4 prj2=modelView*vec4(vertex,1.0);
		prj2.y+=pointSizeIn;
		prj2=projection*prj2;
		gl_PointSize=viewportHei*abs(prj2[1]/prj2[3]-prj1[1]/prj1[3])/2.0;
	}
}
