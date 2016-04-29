#include <fslazywindow.h>
#include <ysgl.h>
#include <ysglslcpp.h>
#include <ysclass.h>


#ifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_EMBEDDED!=0 || TARGET_OS_IPHONE!=0 || TARGET_OS_IPHONE_SIMULATOR!=0
#define GL_CLAMP GL_CLAMP_TO_EDGE
#define __APPLE_IOS__
#endif
#endif


YsArray <unsigned char> TextureRawData(void)
{
	static char textureRawData[16*16*4]=
	{
		"       A        "
		"       A        "
		"      ABA       "
		"      ABA       "
		"     ABBBA      "
		"AAAAABBBBBAAAAA "
		" ABBBBBBBBBBBA  "
		"  ABBBBBBBBBA   "
		"   ABBBBBBBA    "
		"    ABBBBBA     "
		"   ABBBABBBA    "
		"   ABBA ABBA    "
		"  ABBA   ABBA   "
		"  ABA     ABA   "
		"  AA       AA   "
		"  A         A   "
	};

	YsArray <unsigned char> rgba;
	for(auto c : textureRawData)
	{
		switch(c)
		{
		default:
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			break;
		case 'A':
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(255);
			break;
		case 'B':
			rgba.Add(255);
			rgba.Add(255);
			rgba.Add(255);
			rgba.Add(255);
			break;
		}
	}

	return rgba;
}



YsArray <unsigned char> MultiTextureRawData(void)
{
	static char textureRawData[32*32*4]=
	{
		"       A                        "
		"       A               A        "
		"      ABA              A        "
		"      ABA             ABA       "
		"     ABBBA            ABA       "
		"AAAAABBBBBAAAAA      ABBBA      "
		" ABBBBBBBBBBBA       ABBBA      "
		"  ABBBBBBBBBA       ABBBBBA     "
		"   ABBBBBBBA        ABBBBBA     "
		"    ABBBBBA        ABBBBBBBA    "
		"   ABBBABBBA       ABBBBBBBA    "
		"   ABBA ABBA      ABBBBBBBBBA   "
		"  ABBA   ABBA     ABBBBBBBBBA   "
		"  ABA     ABA    ABBBBBBBBBBBA  "
		"  AA       AA    AAAAAAAAAAAAA  "
		"  A         A                   "
		"                                "
		"                                "
		"                                "
		"       A               A        "
		"      ABA            AABAA      "
		"     ABBBA          ABBBBBA     "
		"    ABBBBBA        ABBBBBBBA    "
		"   ABBBBBBBA       ABBBBBBBA    "
		"  ABBBBBBBBBA     ABBBBBBBBBA   "
		" ABBBBBBBBBBBA    ABBBBBBBBBA   "
		"  ABBBBBBBBBA     ABBBBBBBBBA   "
		"   ABBBBBBBA       ABBBBBBBA    "
		"    ABBBBBA        ABBBBBBBA    "
		"     ABBBA          ABBBBBA     "
		"      ABA            AABAA      "
		"       A               A        "
	};

	YsArray <unsigned char> rgba;
	for(auto c : textureRawData)
	{
		switch(c)
		{
		default:
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			break;
		case 'A':
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(0);
			rgba.Add(255);
			break;
		case 'B':
			rgba.Add(255);
			rgba.Add(255);
			rgba.Add(255);
			rgba.Add(255);
			break;
		}
	}

	return rgba;
}



class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	Ys3DDrawingEnvironment drawEnv;
	unsigned int texIdent;
	unsigned int multiTexIdent;
	int nPrevTouch;

public:
	FsLazyWindowApplication();
	virtual void BeforeEverything(int argc,char *argv[]);
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const;
	virtual void Initialize(int argc,char *argv[]);
	virtual void Interval(void);
	virtual void BeforeTerminate(void);
	virtual void Draw(void);
	virtual bool MustTerminate(void) const;
	virtual long long int GetMinimumSleepPerInterval(void) const;
	virtual bool NeedRedraw(void) const;

	enum MODE
	{
		TEST_FLAT_RENDERER,
		TEST_PLAIN_RENDERER,
		TEST_PLAIN_RENDERER_WITH_TEXCOORD,
		TEST_MONOCOLOR_SHADED_RENDERER,
		TEST_VARICOLOR_SHADED_RENDERER,
		TEST_SHADED_RENDERER_WITH_TEXCOORD,
		TEST_MONOCOLOR_BILLBOARD_RENDERER,
		TEST_VARICOLOR_BILLBOARD_RENDERER,
		TEST_MONOCOLOR_MARKER_RENDERER,
		TEST_VARICOLOR_MARKER_RENDERER,
		TEST_FLASH_RENDERER,
		TEST_POINT_SPRITE,
		TEST_UNIFORM_POINT_SIZE,
	};
	MODE mode;
	void TestFlatRenderer(void);
	void TestPlainRenderer(void);
	void TestPlainRendererWithTexCoord(void);
	void TestMonoColorShadedRenderer(void);
	void TestVariColorShadedRenderer(void);
	void TestShadedRendererWithTexCoord(void);
	void TestMonoColorBillboardRenderer(void);
	void TestVariColorBillboardRenderer(void);
	void TestMonOColorMarkerRenderer(void);
	void TestVariColorMarkerRenderer(void);
	void TestFlashRenderer(void);
	void TestPointSpriteRenderer(void);
	void TestUniformPointSize(void);
};

FsLazyWindowApplication::FsLazyWindowApplication()
{
	mode=TEST_FLAT_RENDERER;
	needRedraw=false;
	nPrevTouch=0;
}

/* virtual */ void FsLazyWindowApplication::BeforeEverything(int argc,char *argv[])
{
}
/* virtual */ void FsLazyWindowApplication::GetOpenWindowOption(FsOpenWindowOption &OPT) const
{
	OPT.x0=0;
	OPT.y0=0;
	OPT.wid=1200;
	OPT.hei=800;
}
/* virtual */ void FsLazyWindowApplication::Initialize(int argc,char *argv[])
{
	YsGLSLCreateSharedRenderer();
	YsGLSLCreateSharedBitmapFontRenderer();

	int wid,hei;
	FsGetWindowSize(wid,hei);
	drawEnv.SetProjectionMode(Ys3DDrawingEnvironment::PERSPECTIVE);
	drawEnv.SetAspectRatio((double)wid/(double)hei);
	drawEnv.SetFOVY(YsPi/4.0);
	drawEnv.SetNearFar(1.0,50.0);

	drawEnv.SetViewTarget(YsOrigin());
	drawEnv.SetViewDistance(20.0);

	glGenTextures(1,&texIdent);
    glBindTexture(GL_TEXTURE_2D,texIdent);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexImage2D
        (GL_TEXTURE_2D,
         0,    // Level of detail
         GL_RGBA,
         16, // wid
         16, // hei
         0,    // Border width, but not supported and needs to be 0.
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         TextureRawData());

	glGenTextures(1,&multiTexIdent);
	glBindTexture(GL_TEXTURE_2D,multiTexIdent);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexImage2D
        (GL_TEXTURE_2D,
         0,    // Level of detail
         GL_RGBA,
         32, // wid
         32, // hei
         0,    // Border width, but not supported and needs to be 0.
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         MultiTextureRawData());


	glClearColor(0.8f,0.8f,0.8f,0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	auto nTouch=FsGetNumCurrentTouch();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}
	if(FSKEY_SPACE==key || (0<nPrevTouch && 0==nTouch))
	{
		switch(mode)
		{
		default:
			mode=TEST_FLAT_RENDERER;
			break;
		case TEST_FLAT_RENDERER:
			mode=TEST_PLAIN_RENDERER;
			break;
		case TEST_PLAIN_RENDERER:
			mode=TEST_PLAIN_RENDERER_WITH_TEXCOORD;
			break;
		case TEST_PLAIN_RENDERER_WITH_TEXCOORD:
			mode=TEST_MONOCOLOR_SHADED_RENDERER;
			break;
		case TEST_MONOCOLOR_SHADED_RENDERER:
			mode=TEST_SHADED_RENDERER_WITH_TEXCOORD;
			break;
		case TEST_SHADED_RENDERER_WITH_TEXCOORD:
			mode=TEST_VARICOLOR_SHADED_RENDERER;
			break;
		case TEST_VARICOLOR_SHADED_RENDERER:
			mode=TEST_MONOCOLOR_BILLBOARD_RENDERER;
			break;
		case TEST_MONOCOLOR_BILLBOARD_RENDERER:
			mode=TEST_VARICOLOR_BILLBOARD_RENDERER;
			break;
		case TEST_VARICOLOR_BILLBOARD_RENDERER:
			mode=TEST_MONOCOLOR_MARKER_RENDERER;
			break;
		case TEST_MONOCOLOR_MARKER_RENDERER:
			mode=TEST_VARICOLOR_MARKER_RENDERER;
			break;
		case TEST_VARICOLOR_MARKER_RENDERER:
			mode=TEST_FLASH_RENDERER;
			break;
		case TEST_FLASH_RENDERER:
			mode=TEST_POINT_SPRITE;
			break;
		case TEST_POINT_SPRITE:
			mode=TEST_UNIFORM_POINT_SIZE;
			break;
		};
	}
	nPrevTouch=nTouch;

	const double dt=(double)FsPassedTime()/1000.0;

	auto att=drawEnv.GetViewAttitude();
	att.YawLeft(dt*YsPi/2.0);
	drawEnv.SetViewAttitude(att);

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	switch(mode)
	{
	case TEST_FLAT_RENDERER:
		TestFlatRenderer();
		break;
	case TEST_PLAIN_RENDERER:
		TestPlainRenderer();
		break;
	case TEST_PLAIN_RENDERER_WITH_TEXCOORD:
		TestPlainRendererWithTexCoord();
		break;
	case TEST_MONOCOLOR_SHADED_RENDERER:
		TestMonoColorShadedRenderer();
		break;
	case TEST_VARICOLOR_SHADED_RENDERER:
		TestVariColorShadedRenderer();
		break;
	case TEST_SHADED_RENDERER_WITH_TEXCOORD:
		TestShadedRendererWithTexCoord();
		break;
	case TEST_MONOCOLOR_BILLBOARD_RENDERER:
		TestMonoColorBillboardRenderer();
		break;
	case TEST_VARICOLOR_BILLBOARD_RENDERER:
		TestVariColorBillboardRenderer();
		break;
	case TEST_MONOCOLOR_MARKER_RENDERER:
		TestMonOColorMarkerRenderer();
		break;
	case TEST_VARICOLOR_MARKER_RENDERER:
		TestVariColorMarkerRenderer();
		break;
	case TEST_FLASH_RENDERER:
		TestFlashRenderer();
		break;
	case TEST_POINT_SPRITE:
		TestPointSpriteRenderer();
		break;
	case TEST_UNIFORM_POINT_SIZE:
		TestUniformPointSize();
		break;
	}

	FsSwapBuffers();

	needRedraw=false;
}
/* virtual */ bool FsLazyWindowApplication::MustTerminate(void) const
{
	return FsLazyWindowApplicationBase::MustTerminate();
}
/* virtual */ long long int FsLazyWindowApplication::GetMinimumSleepPerInterval(void) const
{
	return 10;
}
/* virtual */ bool FsLazyWindowApplication::NeedRedraw(void) const
{
	return needRedraw;
}

void FsLazyWindowApplication::TestFlatRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		// YsGLSLFlat3DRenderer renderer; // If generic attribute works, I should be able to use VariColor3DRenderer

		YsGLSLPlain3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);


		renderer.SetUniformColor(0,0,1,1);
		float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		renderer.DrawVtx(YsGL::TRIANGLE_FAN,4,square);
	}


	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		// renderer.DrawString(60,100,"Testing Flat Renderer");
		renderer.DrawString(60,100,"Testing Plain Renderer with Color as Generic Attribute (Must see a blue square!)");
	}
}

void FsLazyWindowApplication::TestPlainRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLPlain3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);


		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float color[16]=
		{
			1,0,0,1,
			0,1,0,1,
			0,0,1,1,
			1,1,0,0
		};
		renderer.DrawVtxCol(YsGL::TRIANGLE_FAN,4,square,color);
	}

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Plain Renderer");
	}
}

void FsLazyWindowApplication::TestPlainRendererWithTexCoord(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLPlain3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);


		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float color[16]=
		{
			1,0,0,1,
			0,1,0,1,
			0,0,1,1,
			1,1,0,0
		};
		const float texCoord[8]=
		{
			0,1,
			1,1,
			1,0,
			0,0
		};

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,texIdent);

		renderer.SetTextureType(YSGLSL_TEX_TYPE_ATTRIBUTE);
		renderer.SetTextureIdent(0);

		renderer.DrawVtxTexCoordCol(YsGL::TRIANGLE_FAN,4,square,texCoord,color);

		renderer.SetTextureType(YSGLSL_TEX_TYPE_NONE);
	}

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Plain Renderer with TexCoord");
	}
}

void FsLazyWindowApplication::TestMonoColorShadedRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		// YsGLSLMonoColorShaded3DRenderer renderer;

		YsGLSLShaded3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		const float color[4]=
		{
			0,0,1,1
		};
		renderer.SetUniformColor(color);

		const float lightDir[3]=
		{
			0,0,1
		};
		renderer.SetLightDirectionInCameraCoordinate(0,lightDir);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float normal[12]=
		{
			0,0,1,
			0,0,1,
			0,0,1,
			0,0,1,
		};
		renderer.DrawVtxNom(YsGL::TRIANGLE_FAN,4,square,normal);
	}
	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		// renderer.DrawString(60,100,"Testing MonoColor Shaded Renderer");
		renderer.DrawString(60,100,"Testing Shaded Renderer with Color as Generic Attribute (Must see a Blue Square)");
	}
}

void FsLazyWindowApplication::TestVariColorShadedRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLShaded3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		const float lightDir[3]=
		{
			0,0,1
		};
		renderer.SetLightDirectionInCameraCoordinate(0,lightDir);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float normal[12]=
		{
			0,0,1,
			0,0,1,
			0,0,1,
			0,0,1,
		};
		const float color[16]=
		{
			0,0,1,1,
			0,0,1,1,
			1,0,1,1,
			1,0,1,1
		};

		renderer.DrawVtxNomCol(YsGL::TRIANGLE_FAN,4,square,normal,color);
	}
	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Shaded Renderer");
	}
}

void FsLazyWindowApplication::TestShadedRendererWithTexCoord(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLShaded3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		const float lightDir[3]=
		{
			0,0,1
		};
		renderer.SetLightDirectionInCameraCoordinate(0,lightDir);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float normal[12]=
		{
			0,0,1,
			0,0,1,
			0,0,1,
			0,0,1,
		};
		const float color[16]=
		{
			0,0,1,1,
			0,0,1,1,
			1,0,1,1,
			1,0,1,1
		};
		const float texCoord[8]=
		{
			0,1,
			1,1,
			1,0,
			0,0
		};

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,texIdent);

		renderer.SetTextureType(YSGLSL_TEX_TYPE_ATTRIBUTE);
		renderer.SetTextureIdent(0);

		renderer.DrawVtxTexCoordNomCol(YsGL::TRIANGLE_FAN,4,square,texCoord,normal,color);

		renderer.SetTextureType(YSGLSL_TEX_TYPE_NONE);
	}
	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Shaded Renderer with TexCoord");
	}
}

void FsLazyWindowApplication::TestMonoColorBillboardRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLBillboard3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,texIdent);

		renderer.SetBillboardTransformationType(YSGLSL_TEX_BILLBOARD_PERS);
		renderer.SetTextureIdent(0);

		const float vtx[12]=
		{
			-5.0,0,0,
			-5.0,0,0,
			-5.0,0,0,
			-5.0,0,0
		};
		const float vOffset[12]=
		{
			-2.0,-2.0,0,
			 2.0,-2.0,0,
			 2.0, 2.0,0,
			-2.0, 2.0,0,
		};
		const float texCoord[8]=
		{
			 1, 1,
			-1, 1,
			-1,-1,
			 1,-1,
		};

		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawVtxVoffsetTexCoord(YsGL::TRIANGLE_FAN,4,vtx,vOffset,texCoord);
	}

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Billboard Renderer with Color as Generic Attribute");
	}
}

void FsLazyWindowApplication::TestVariColorBillboardRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLBillboard3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,texIdent);

		renderer.SetBillboardTransformationType(YSGLSL_TEX_BILLBOARD_PERS);
		renderer.SetTextureIdent(0);

		const float vtx[12]=
		{
			-5.0,0,0,
			-5.0,0,0,
			-5.0,0,0,
			-5.0,0,0
		};
		const float vOffset[12]=
		{
			-2.0,-2.0,0,
			 2.0,-2.0,0,
			 2.0, 2.0,0,
			-2.0, 2.0,0,
		};
		const float col[16]=
		{
			0,0,1,1,
			0,0,1,1,
			0,1,1,1,
			0,1,1,1,
		};
		const float texCoord[8]=
		{
			 1, 1,
			-1, 1,
			-1,-1,
			 1,-1,
		};

		renderer.DrawVtxColVoffsetTexCoord(YsGL::TRIANGLE_FAN,4,vtx,col,vOffset,texCoord);
	}

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Billboard Renderer");
	}
}

void FsLazyWindowApplication::TestMonOColorMarkerRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLMarker3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		const float col[4]=
		{
			0,0,1,1
		};
		renderer.SetUniformColor(col);
		renderer.SetMarkerType(YSGLSL_MARKER_TYPE_STAR);
		renderer.SetMarkerDimension(20);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};

#ifndef __APPLE_IOS__
		glEnable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glEnable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
		renderer.DrawVtx(YsGL::POINTS,4,square);
#ifndef __APPLE_IOS__
		glDisable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glDisable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
	}

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Marker Renderer with Color as Generic Attribute");
	}
}

void FsLazyWindowApplication::TestVariColorMarkerRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLMarker3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		renderer.SetMarkerType(YSGLSL_MARKER_TYPE_STAR);
		renderer.SetMarkerDimension(20);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float col[16]=
		{
			0,0,1,1,
			0,1,0,1,
			1,0,0,1,
			1,0,1,1,
		};

#ifndef __APPLE_IOS__
		glEnable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glEnable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
        renderer.DrawVtxCol(YsGL::POINTS,4,square,col);
#ifndef __APPLE_IOS__
		glDisable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glDisable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
    }

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Marker Renderer");
	}
}

void FsLazyWindowApplication::TestFlashRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLFlash3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetPointSizeMode(YSGLSL_POINTSPRITE_SIZE_IN_3DSPACE);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float col[16]=
		{
			0,0,1,1,
			0,1,0,1,
			1,0,0,1,
			1,0,1,1,
		};
		const float flashSize[4]=
		{
			5.0F,5.0F,5.0F,5.0F,
		};

#ifndef __APPLE_IOS__
		glEnable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glEnable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
        renderer.DrawVtxColPointSize(YsGL::POINTS,4,square,col,flashSize);
#ifndef __APPLE_IOS__
		glDisable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glDisable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
    }

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Flash Renderer");
	}
}

void FsLazyWindowApplication::TestPointSpriteRenderer(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLPointSprite3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);

		renderer.SetAlphaCutOff(0.5f);

		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetPointSizeMode(YSGLSL_POINTSPRITE_SIZE_IN_3DSPACE);
		renderer.SetPointSpriteTextureCoordRange(0.5);
		renderer.SetTextureType(YSGLSL_TEX_TYPE_BILLBOARD);
		renderer.SetTextureIdent(0);

		const float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
		const float col[16]=
		{
			0,0,1,1,
			0,1,0,1,
			1,0,0,1,
			1,0,1,1,
		};
		const float texCoord[8]=
		{
			0.0F,0.0F,
			0.5F,0.0F,
			0.0F,0.5F,
			0.5F,0.5F,
		};
		const float pointSize[4]=
		{
			5.0F,5.0F,5.0F,5.0F,
		};

	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,multiTexIdent);

#ifndef __APPLE_IOS__
		glEnable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glEnable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
        renderer.DrawVtxTexCoordColPointSize(YsGL::POINTS,4,square,texCoord,col,pointSize);
#ifndef __APPLE_IOS__
		glDisable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
		glDisable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
    }

	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		renderer.DrawString(60,100,"Testing Point-Sprite Renderer");
	}
}

void FsLazyWindowApplication::TestUniformPointSize(void)
{
	int wid,hei;
	FsGetWindowSize(wid,hei);

	{
		YsGLSLPlain3DRenderer renderer;

		auto projMat=drawEnv.GetProjectionMatrix();
		auto viewMat=drawEnv.GetViewMatrix();

		float mat[16];
		projMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetProjection(mat);
		viewMat.GetOpenGlCompatibleMatrix(mat);
		renderer.SetModelView(mat);


		renderer.SetUniformPointSize(40.0f);
		renderer.SetUniformColor(0,0,1,1);
		float square[12]=
		{
			-5,-5, 0,
			 5,-5, 0,
			 5, 5, 0,
			-5, 5, 0,
		};
#ifdef GL_PROGRAM_POINT_SIZE
		glEnable(GL_PROGRAM_POINT_SIZE);
#endif
		renderer.DrawVtx(YsGL::POINTS,4,square);
#ifdef GL_PROGRAM_POINT_SIZE
		glDisable(GL_PROGRAM_POINT_SIZE);
#endif
	}


	{
		YsGLSLBitmapFontRendererClass renderer;
		renderer.RequestFontSize(24,24);
		renderer.SetViewportDimension((float)wid,(float)hei);
		renderer.SetUniformColor(0,0,1,1);
		// renderer.DrawString(60,100,"Testing Flat Renderer");
		renderer.DrawString(60,100,"Testing Plain Renderer with Uniform Point Size");
	}
}

static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}
