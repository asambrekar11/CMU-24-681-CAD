
LOWP vec4 YsGLSLRainbowColor(HIGHP float t)
{
	// 0     0.25  0.5    0.75    1
	// Blue->Cyan->Green->Yellow->Red

	HIGHP float tt;
	if(t<0.0)
	{
		return vec4(0,0,1,1);
	}
	else if(t<0.25)
	{
		tt=t/0.25;
		return vec4(0,tt,1,1);
	}
	else if(t<0.5)
	{
		tt=(t-0.25)/0.25;
		return vec4(0,1,1.0-tt,1);
	}
	else if(t<0.75)
	{
		tt=(t-0.5)/0.25;
		return vec4(tt,1,0,1);
	}
	else if(t<1.0)
	{
		tt=(t-0.75)/0.25;
		return vec4(1,1.0-tt,0,1);
	}
	else
	{
		return vec4(1,0,0,1);
	}
}

