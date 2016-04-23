#include "ysgl.h"
#include <math.h>


void YsGLCopyBitmap(int w,int h,GLfloat *dst,const GLfloat *src)
{
	int i;
	for(i=0; i<w*h*4; ++i)
	{
		dst[i]=src[i];
	}
}

void YsGLCopyUCharBitmapToGLFloatBitmap(int w,int h,GLfloat *dst,const unsigned char *src)
{
	int i;
	for(i=0; i<w*h*4; ++i)
	{
		dst[i]=(GLfloat)src[i]/(GLfloat)255.0;
	}
}

void YsGLApplySphereMap(
    int w,int h,GLfloat *rgba,
    GLfloat ambientColor[],  // r,g,b
    int nLight,
	GLint lightEnabled[],
	GLfloat lightPos[],
	GLfloat lightColor[],
	int useSpecular,
	GLfloat specularColor[], // r,g,b
	GLfloat specularExponent)
{
	int y;
	for(y=0; y<h; ++y)
	{
		int x;
		GLfloat nom[3];
		nom[1]=(GLfloat)y/(GLfloat)h;
		nom[1]=(nom[1]*2.0f)-1.0f;

		for(x=0; x<w; ++x)
		{
			GLfloat d;
			GLfloat *pix=rgba+(y*w+x)*4;
			nom[0]=(GLfloat)x/(GLfloat)w;
			nom[0]=(nom[0]*2.0f)-1.0f;

			d=nom[0]*nom[0]+nom[1]*nom[1];
			if(1.0>d)
			{
				int lightNo;
				GLfloat accumColor[3];
				accumColor[0]=pix[0]*ambientColor[0],
				accumColor[1]=pix[1]*ambientColor[1],
				accumColor[2]=pix[2]*ambientColor[2],
				nom[2]=(GLfloat)sqrt(1.0f-d);

				for(lightNo=0; lightNo<nLight; lightNo++)
				{
					if(0!=lightEnabled[lightNo])
					{
						GLfloat dot=nom[0]*lightPos[lightNo*4]+nom[1]*lightPos[lightNo*4+1]+nom[2]*lightPos[lightNo*4+2];
						if(0.0f<=dot)
						{
							accumColor[0]+=dot*lightColor[lightNo*3];
							accumColor[1]+=dot*lightColor[lightNo*3+1];
							accumColor[2]+=dot*lightColor[lightNo*3+2];

							if(0!=useSpecular)
							{
								GLfloat mid[3],l,dot;
								mid[0]=lightPos[lightNo*4];
								mid[1]=lightPos[lightNo*4+2];
								mid[2]=lightPos[lightNo*4+3]+1.0f;
								l=(GLfloat)sqrt(mid[0]*mid[0]+mid[1]*mid[1]+mid[2]*mid[2]);
								mid[0]/=l;
								mid[1]/=l;
								mid[2]/=l;
								dot=mid[0]*nom[0]+mid[1]*nom[1]+mid[2]*nom[2];
								if(0.0f<dot)
								{
									GLfloat specular;
									specular=(GLfloat)pow(dot,specularExponent);

									accumColor[0]+=specular*specularColor[lightNo*3];
									accumColor[1]+=specular*specularColor[lightNo*3+1];
									accumColor[2]+=specular*specularColor[lightNo*3+2];
								}
							}
						}
					}
				}

				pix[0]=(accumColor[0]<1.0f ? accumColor[0] : 1.0f);
				pix[1]=(accumColor[1]<1.0f ? accumColor[1] : 1.0f);
				pix[2]=(accumColor[2]<1.0f ? accumColor[2] : 1.0f);
			}
			else
			{
				pix[0]=0.0f;
				pix[1]=0.0f;
				pix[2]=0.0f;
				pix[3]=0.0f;
			}
		}
	}
}

