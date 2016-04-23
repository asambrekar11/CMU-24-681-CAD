#ifndef YSBITMAPCONTOUR_IS_INCLUDED
#define YSBITMAPCONTOUR_IS_INCLUDED
/* { */

#include <ysclass.h>
#include <ysbitmap.h>


/*

  Vertx(x,y)
      |
      V
      +-----U------+-->X
      |            |
      |   Block    |
      L   (x,y)    |
      |            |
      |            |
      +------------+
      |
      V
       Y

  U: s=0
  L: s=1

  Edge (x,y,s)

  (x,y,0) connects vertex (x,y)-(x+1,y)
  (x,y,1) connects vertex (x,y)-(x,y+1)

*/



class YsBitmapContourExtraction
{
private:
	const YsBitmap *bmpPtr;
	YsFixedLengthHashTable <int,3> visited;
	YsArray <YsArray <YsVec2i> > contourArray;

public:
	class Wall
	{
	public:
		YsVec2i xy;
		int s;

		void Set(int x,int y,int s)
		{
			this->xy.Set(x,y);
			this->s=s;
		}
	};

	YsBitmapContourExtraction(const YsBitmap &bmp);

	/*! Bmp must be the same bitmap passed to the constructor. */
	YSRESULT RemovePointConnection(YsBitmap &Bmp);

	void CleanUp(void);
	void ExtractContour(void);


	/*! This function re-samples contours so that total number of edges will be close to NEdge. */
	void Resample(int NEdge);

	const YsArray <YsArray <YsVec2i> > GetContour(void) const;

	YsArray <YsVec2i> TrackBoundary(int x,int y,int s);

	YSBOOL WallIsBoundary(YsVec2i pos,int s) const;
	YSBOOL WallIsBoundary(int x,int y,int s) const;

	YSBOOL IsInside(YsVec2i pos) const;
	virtual YSBOOL IsInside(const unsigned char rgba[4]) const;

	static inline void RotateLeft(YsVec2i &vec)
	{
		if(0<vec.y())
		{
			vec.SubX(1);
			if(vec.x()<-1)
			{
				vec.SetX(-1);
				vec.SetY(0);
			}
		}
		else if(vec.x()<0)
		{
			vec.SubY(1);
			if(vec.y()<-1)
			{
				vec.SetX(0);
				vec.SetY(-1);
			}
		}
		else if(vec.y()<0)
		{
			vec.AddX(1);
			if(1<vec.x())
			{
				vec.SetX(1);
				vec.SetY(0);
			}
		}
		else
		{
			vec.AddY(1);
			if(1<vec.y())
			{
				vec.SetX(0);
				vec.SetY(1);
			}
		}
	}
	static inline void RotateRight(YsVec2i &vec)
	{
		if(0<vec.y())
		{
			vec.AddX(1);
			if(1<vec.x())
			{
				vec.SetX(1);
				vec.SetY(0);
			}
		}
		else if(0<vec.x())
		{
			vec.SubY(1);
			if(vec.y()<-1)
			{
				vec.SetX(0);
				vec.SetY(-1);
			}
		}
		else if(vec.y()<0)
		{
			vec.SubX(1);
			if(vec.x()<-1)
			{
				vec.SetX(-1);
				vec.SetY(0);
			}
		}
		else
		{
			vec.AddY(1);
			if(1<vec.y())
			{
				vec.SetX(0);
				vec.SetY(1);
			}
		}
	}
};

/* } */
#endif
