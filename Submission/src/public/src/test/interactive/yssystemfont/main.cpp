#include <fslazywindow.h>
#include <ysglcpp.h>
#include <ysglslcpp.h>
#include <yssystemfont.h>

class FsLazyWindowApplication : public FsLazyWindowApplicationBase
{
protected:
	bool needRedraw;
	unsigned int texIdent;
	YsSystemFontCache sysFont;
	Ys3DDrawingEnvironment drawEnv;
	float aspect;

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
};

FsLazyWindowApplication::FsLazyWindowApplication()
{
	needRedraw=false;
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	unsigned char fgCol[3]={0,0,255},bgCol[3]={0,0,0};

	sysFont.RequestDefaultFontWithHeight(32);
	auto txtBmp=sysFont.MakeRGBABitmap(
	   L"This program is for testing YsSystemFont library.\n"
	   L"012345ABCDE!@#$%",
	   fgCol,bgCol,YSFALSE);

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
         txtBmp.Width(),  // wid
         txtBmp.Height(), // hei
         0,    // Border width, but not supported and needs to be 0.
         GL_RGBA,
         GL_UNSIGNED_BYTE,
         txtBmp.Bitmap());

	aspect=(double)txtBmp.Width()/(double)txtBmp.Height();
}
/* virtual */ void FsLazyWindowApplication::Interval(void)
{
	auto key=FsInkey();
	if(FSKEY_ESC==key)
	{
		SetMustTerminate(true);
	}

	const double dt=(double)FsPassedTime()/1000.0;

	auto att=drawEnv.GetViewAttitude();
	att.YawLeft(dt*YsPi/8.0);
	drawEnv.SetViewAttitude(att);

	needRedraw=true;
}
/* virtual */ void FsLazyWindowApplication::BeforeTerminate(void)
{
}
/* virtual */ void FsLazyWindowApplication::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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

		glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D,texIdent);
		renderer.SetTextureIdent(0);
		renderer.SetTextureType(YSGLSL_TEX_TYPE_ATTRIBUTE);


		float square[12]=
		{
			-15.0f,-15.0f/aspect, 0,
			 15.0f,-15.0f/aspect, 0,
			 15.0f, 15.0f/aspect, 0,
			-15.0f, 15.0f/aspect, 0,
		};
		float color[16]=
		{
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
			0,0,1,1,
		};
		float texCoord[8]=
		{
			0,1,
			1,1,
			1,0,
			0,0
		};
		renderer.DrawVtxTexCoordCol(YsGL::TRIANGLE_FAN,4,square,texCoord,color);
		// renderer.DrawVtxCol(YsGL::TRIANGLE_FAN,4,square,color);
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


static FsLazyWindowApplication *appPtr=nullptr;

/* static */ FsLazyWindowApplicationBase *FsLazyWindowApplicationBase::GetApplication(void)
{
	if(nullptr==appPtr)
	{
		appPtr=new FsLazyWindowApplication;
	}
	return appPtr;
}
