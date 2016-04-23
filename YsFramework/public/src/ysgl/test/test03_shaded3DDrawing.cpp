/* ////////////////////////////////////////////////////////////

File Name: test03_shaded3DDrawing.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <math.h>
#include <fssimplewindow.h>
#include <ysgl.h>

int MakeSphere(GLfloat vtx[],GLfloat nom[],GLfloat col[],int nh,int nv)
{
	int nVtx=0;
	for(int v=0; v<nv; v++)
	{
		const double v0=-YSGLPI/2.0+YSGLPI*(double)v/(double)nv;
		const double v1=-YSGLPI/2.0+YSGLPI*(double)(v+1)/(double)nv;

		for(int h=0; h<nh; h++)
		{
			const int colIndex=1+(v*2/nv)+(h*4/nh)*2;

			const double h0=YSGLPI*2.0*(double)h/(double)nh;
			const double h1=YSGLPI*2.0*(double)(h+1)/(double)nh;

			vtx[nVtx*3  ]=(GLfloat)(cos(v0)*cos(h0));
			vtx[nVtx*3+1]=(GLfloat)(sin(v0));
			vtx[nVtx*3+2]=(GLfloat)(cos(v0)*sin(h0));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;

			vtx[nVtx*3  ]=(GLfloat)(cos(v0)*cos(h1));
			vtx[nVtx*3+1]=(GLfloat)(sin(v0));
			vtx[nVtx*3+2]=(GLfloat)(cos(v0)*sin(h1));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;

			vtx[nVtx*3  ]=(GLfloat)(cos(v1)*cos(h0));
			vtx[nVtx*3+1]=(GLfloat)(sin(v1));
			vtx[nVtx*3+2]=(GLfloat)(cos(v1)*sin(h0));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;

			vtx[nVtx*3  ]=(GLfloat)(cos(v0)*cos(h1));
			vtx[nVtx*3+1]=(GLfloat)(sin(v0));
			vtx[nVtx*3+2]=(GLfloat)(cos(v0)*sin(h1));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;

			vtx[nVtx*3  ]=(GLfloat)(cos(v1)*cos(h1));
			vtx[nVtx*3+1]=(GLfloat)(sin(v1));
			vtx[nVtx*3+2]=(GLfloat)(cos(v1)*sin(h1));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;

			vtx[nVtx*3  ]=(GLfloat)(cos(v1)*cos(h0));
			vtx[nVtx*3+1]=(GLfloat)(sin(v1));
			vtx[nVtx*3+2]=(GLfloat)(cos(v1)*sin(h0));
			col[nVtx*4  ]=(GLfloat)(colIndex&1);
			col[nVtx*4+1]=(GLfloat)((colIndex>>1)&1);
			col[nVtx*4+2]=(GLfloat)((colIndex>>2)&1);
			col[nVtx*4+3]=1;
			nVtx++;
		}
	}

	for(int i=0; i<nVtx; i++)
	{
		nom[i*3  ]=vtx[i*3  ];
		nom[i*3+1]=vtx[i*3+1];
		nom[i*3+2]=vtx[i*3+2];
	}

	return nVtx;
}


int main(void)
{
	GLfloat vtx[3*3*2*8*16],nom[3*3*2*8*16],col[3*4*2*8*16];
	int nVtx=MakeSphere(vtx,nom,col,16,8);

	FsOpenWindow(16,16,800,600,1);

	YsGLSL3DRenderer *variColorPerVtxShadingRenderer=YsGLSLCreateVariColorPerVtxShading3DRenderer();
	YsGLSL3DRenderer *monoColorPerVtxShadingRenderer=YsGLSLCreateMonoColorPerVtxShading3DRenderer();
	YsGLSL3DRenderer *monoColorPerPixShadingRenderer=YsGLSLCreateMonoColorPerPixShading3DRenderer();
	YsGLSL3DRenderer *variColorPerPixShadingRenderer=YsGLSLCreateVariColorPerPixShading3DRenderer();
	YsGLSL3DRenderer *renderer3d=monoColorPerVtxShadingRenderer;

	// Just error check
	YsGLSLCreateMonoColorPerPixShadingWithTexCoord3DRenderer();
	// Just error check
	YsGLSLCreateVariColorPerPixShadingWithTexCoord3DRenderer();

	printf("SPACE   Switch between Per Vertex and Per Pixel renderers.\n");


	double rot=0.0;

	for(int key=FSKEY_NULL; FSKEY_ESC!=key; FsSleep(10))
	{
		FsPollDevice();
		key=FsInkey();

		switch(key)
		{
		case FSKEY_SPACE:
			if(renderer3d==monoColorPerVtxShadingRenderer)
			{
				printf("monoColorPerPixShadingRenderer\n");
				renderer3d=monoColorPerPixShadingRenderer;
			}
			else if(renderer3d==monoColorPerPixShadingRenderer)
			{
				printf("variColorPerVtxShadingRenderer\n");
				renderer3d=variColorPerVtxShadingRenderer;
			}
			else if(renderer3d==variColorPerVtxShadingRenderer)
			{
				printf("variColorPerPixShadingRenderer\n");
				renderer3d=variColorPerPixShadingRenderer;
			}
			else
			{
				printf("monoColorPerVtxShadingRenderer\n");
				renderer3d=monoColorPerVtxShadingRenderer;
			}
			break;
		}


		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		int wid,hei;
		FsGetWindowSize(wid,hei);

		const double aspect=(double)wid/(double)hei;

		GLfloat modelview[16],projection[16],tfm[16];
		YsGLMakePerspectivefv(projection,YSGLPI/6.0,aspect,1.0,50.0);

		YsGLMakeIdentityfv(modelview);
		YsGLMakeTranslationfv(tfm,0.0,0.0,-15.0);
		YsGLMultMatrixfv(modelview,modelview,tfm);
		YsGLMakeXZRotationfv(tfm,rot);
		YsGLMultMatrixfv(modelview,modelview,tfm);
		YsGLMakeScalingfv(tfm,3.0,3.0,3.0);
		YsGLMultMatrixfv(modelview,modelview,tfm);


		YsGLSLUse3DRenderer(monoColorPerVtxShadingRenderer);
		YsGLSLSet3DRendererProjectionfv(monoColorPerVtxShadingRenderer,projection);
		YsGLSLSet3DRendererModelViewfv(monoColorPerVtxShadingRenderer,modelview);
		YsGLSLEndUse3DRenderer(monoColorPerVtxShadingRenderer);

		YsGLSLUse3DRenderer(variColorPerVtxShadingRenderer);
		YsGLSLSet3DRendererProjectionfv(variColorPerVtxShadingRenderer,projection);
		YsGLSLSet3DRendererModelViewfv(variColorPerVtxShadingRenderer,modelview);
		YsGLSLEndUse3DRenderer(variColorPerVtxShadingRenderer);

		YsGLSLUse3DRenderer(monoColorPerPixShadingRenderer);
		YsGLSLSet3DRendererProjectionfv(monoColorPerPixShadingRenderer,projection);
		YsGLSLSet3DRendererModelViewfv(monoColorPerPixShadingRenderer,modelview);
		YsGLSLEndUse3DRenderer(monoColorPerPixShadingRenderer);

		YsGLSLUse3DRenderer(variColorPerPixShadingRenderer);
		YsGLSLSet3DRendererProjectionfv(variColorPerPixShadingRenderer,projection);
		YsGLSLSet3DRendererModelViewfv(variColorPerPixShadingRenderer,modelview);
		YsGLSLEndUse3DRenderer(variColorPerPixShadingRenderer);


		YsGLSLUse3DRenderer(renderer3d);

		GLfloat color[4]={0,0,1,1};
		YsGLSLSet3DRendererUniformColorfv(renderer3d,color);

		YsGLSLDrawPrimitiveVtxNomColfv(renderer3d,GL_TRIANGLES,nVtx,vtx,nom,col);

		YsGLSLEndUse3DRenderer(renderer3d);



		FsSwapBuffers();

		rot+=YSGLPI/30.0;
	}

	YsGLSLDelete3DRenderer(monoColorPerVtxShadingRenderer);
	YsGLSLDelete3DRenderer(variColorPerVtxShadingRenderer);
	YsGLSLDelete3DRenderer(monoColorPerPixShadingRenderer);

	return 0;
}
