#ifndef YSBITMAPDRAWING_IS_INCLUDED
#define YSBITMAPDRAWING_IS_INCLUDED
/* { */

#include <ysclass.h>
#include <ysbitmap.h>

class YsBitmapDrawing
{
public:
	class Option
	{
	public:
		unsigned char rgba[4];
		Option()
		{
			rgba[0]=0;
			rgba[1]=0;
			rgba[2]=0;
			rgba[3]=255;
		}
		void SetColor(const unsigned char rgba[4])
		{
			this->rgba[0]=rgba[0];
			this->rgba[1]=rgba[1];
			this->rgba[2]=rgba[2];
			this->rgba[3]=rgba[3];
		}
	};
	class LineOption : public Option
	{
	public:
		unsigned int thickness;
		LineOption()
		{
			thickness=1;
		}
	};

private:
	YsBitmap *bmpPtr;

	int minPlgItscY,maxPlgItscY;
	YsArray <int> plgItscBuf;

public:
	YsBitmapDrawing(YsBitmap &bmp);

	void Clear(const unsigned char rgba[]);

	void DrawLine(int x0,int y0,int x1,int y1,const LineOption &option);
	void DrawLine(int x0,int y0,int x1,int y1,const unsigned char rgba[]);
	void DrawLineNoClip(int x0,int y0,int x1,int y1,const unsigned char rgba[]);

	void DrawPolygon(YSSIZE_T n,const int xy[],const unsigned char rgba[]);

	void DrawCircle(int x,int y,int r,const unsigned char rgba[],YSBOOL fill,int nDiv=64);

	void FillPolygonLineIntersection(const unsigned char rgba[4]);

	/*! This function returns last VY of the polygon, taking bitmap height into account.
	    If the polygon is completely out of the bitmap's Y range, this function returns 0.
	*/
	int GetPolygonLastVy(YSSIZE_T n,const int xy[]) const;

private:
	void CleanUpPolygonLineIntersection(void);
	void AddPolygonLineIntersectionLine(int x0,int y0,int x1,int y1,int &prevVy);
	void ErasePolygonLineIntersectionForOneY(int y);
	void MakePolygonLineIntersectionBufferForXSymmetric(int x);

public:
	inline void SetPixel(int x,int y,const unsigned char rgba[])
	{
		bmpPtr->SetRGBA(x,y,rgba[0],rgba[1],rgba[2],rgba[3]);
	}

private:
	inline void AddPolygonIntersection(int x,int y)
	{
		YsMakeSmaller(x,bmpPtr->GetWidth()-1);
		YsMakeGreater(x,0);

		YsMakeSmaller(minPlgItscY,y);
		YsMakeGreater(maxPlgItscY,y);

		if(plgItscBuf[y*2]<0)
		{
			plgItscBuf[y*2]=x;
		}
		else
		{
			if(x<plgItscBuf[y*2])
			{
				plgItscBuf[y*2+1]=plgItscBuf[y*2];
				plgItscBuf[y*2]  =x;
			}
			else
			{
				plgItscBuf[y*2+1]=x;
			}
		}
	}
};


/* } */
#endif
