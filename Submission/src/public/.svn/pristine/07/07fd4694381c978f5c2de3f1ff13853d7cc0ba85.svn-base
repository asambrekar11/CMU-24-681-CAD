#import <Cocoa/Cocoa.h>
#include <stdlib.h>

extern unsigned char *YsMacOSXSystemFont_AllocBuffer(size_t bufSize);
// Why this function?
// Buffer needs to be allocated using the new operator, or the buffer cannot
// be released by the delete operator.  Therefore, allocation needs to be
// done in C++ world.


typedef struct
{
#if !__has_feature(objc_arc)
	NSAutoreleasePool *pool;
#endif

	NSFontManager *fontManager;
	// Font manager is not used in the v1.0
	NSFont *font;
	NSMutableParagraphStyle *parag;
	NSMutableDictionary *attrib;

	unsigned int defHeight;
} YsMacOSXSystemFontContext;

void *YsMacOSXSystemFont_CreateContext(void)
{
	YsMacOSXSystemFontContext *ctxt;
	ctxt=(YsMacOSXSystemFontContext *)malloc(sizeof(YsMacOSXSystemFontContext));
	ctxt->pool=NULL;
	ctxt->fontManager=NULL;
	ctxt->font=NULL;
	ctxt->parag=NULL;
	ctxt->attrib=NULL;
	ctxt->defHeight=20;

#if !__has_feature(objc_arc)
	ctxt->pool=[[NSAutoreleasePool alloc] init];
#endif

	ctxt->fontManager=[NSFontManager sharedFontManager];

	const float fontSize=(float)ctxt->defHeight;
	ctxt->font=[NSFont systemFontOfSize:fontSize];

	ctxt->parag=[[NSMutableParagraphStyle alloc] init];
	[ctxt->parag setMaximumLineHeight:fontSize];

	ctxt->attrib=[NSMutableDictionary dictionary];
	[ctxt->attrib setObject:ctxt->font forKey:NSFontAttributeName];
	[ctxt->attrib setObject:[NSNumber numberWithFloat:0.0] forKey:NSKernAttributeName];
	[ctxt->attrib setObject:ctxt->parag forKey:NSParagraphStyleAttributeName];
	[ctxt->attrib setObject:[NSColor whiteColor] forKey:NSForegroundColorAttributeName];
	[ctxt->attrib setObject:[NSColor blackColor] forKey:NSBackgroundColorAttributeName];

	return ctxt;
}

void YsMacOSXSystemFont_FreeContext(void *ctxtPtr)
{
	YsMacOSXSystemFontContext *ctxt=(YsMacOSXSystemFontContext *)ctxtPtr;

#if !__has_feature(objc_arc)
	[ctxt->pool release];
	[ctxt->parag release];
	ctxt->pool=nil;
#endif

	// Can Objective-C can really auto-release ctxt->font and ctxt->attrib?
	ctxt->font=nil; // Does this invoke auto release?
	ctxt->attrib=nil; // Does this invoke auto release?
	ctxt->parag=nil;

	free(ctxtPtr);
}

int YsMacOSXSystemFont_SelectSystemFontWithHeight(void *ctxtPtr,unsigned int height)
{
	int res=0;
	YsMacOSXSystemFontContext *ctxt=(YsMacOSXSystemFontContext *)ctxtPtr;

	const float fontSize=(float)height;
	NSFont *newFont=[NSFont systemFontOfSize:fontSize];
	if(NULL!=newFont)
	{
		if(NULL!=ctxt->attrib)
		{
			[ctxt->attrib setObject:newFont forKey:NSFontAttributeName];
		}
		if(NULL!=ctxt->parag)
		{
			[ctxt->parag setMaximumLineHeight:fontSize];
		}
		
		if(NULL!=ctxt->font)
		{
		#if !__has_feature(objc_arc)
			// [ctxt->font release];
			// 2015/04/05 Looks like [NSFont systemFontOfSize] shouldn't be released, or I get over-release warning.
		#endif
			ctxt->font=nil;
		}
		ctxt->font=newFont;
		res=1;
	}

	return res;
}

int YsMacOSXSystemFont_SelectSystemFont(void *ctxtPtr)
{
	YsMacOSXSystemFontContext *ctxt=(YsMacOSXSystemFontContext *)ctxtPtr;
	return YsMacOSXSystemFont_SelectSystemFontWithHeight(ctxtPtr,ctxt->defHeight);
}

unsigned char *YsMacOSXSystemFont_RGBABitmap(
	void *ctxtPtr,
	unsigned int *wid,unsigned int *hei,unsigned int *dstBytePerLine,
	const wchar_t str[],unsigned int dstBitPerPixel,
	const unsigned char fgCol[3],const unsigned char bgCol[3],
	int reverse)
{
	if(NULL==str)
	{
		*wid=0;
		*hei=0;
		*dstBytePerLine=0;
		return NULL;
	}


	YsMacOSXSystemFontContext *ctxt=(YsMacOSXSystemFontContext *)ctxtPtr;

	wchar_t tst=0x0001;
	unsigned char *tstByte=(unsigned char *)&tst;
	NSStringEncoding encoding;

	if(0!=tstByte[0])
	{
		encoding=NSUTF16LittleEndianStringEncoding;
	}
	else
	{
		encoding=NSUTF16BigEndianStringEncoding;
	}

	*wid=0;
	*hei=0;

	int i=0,i0=0;
	for(;;)
	{
		if(0==str[i] || '\n'==str[i])
		{
			NSString *rawString=[[NSString alloc]
									initWithBytes:str+i0 
									length:(i-i0)*sizeof(wchar_t) 
									encoding:encoding];

			NSAttributedString *string=[[NSAttributedString alloc]
								 initWithString:rawString attributes:ctxt->attrib];

			NSSize strSize=[string size];
			*wid=(*wid<strSize.width ? strSize.width : *wid);
			*hei+=strSize.height;

		#if !__has_feature(objc_arc)
			[rawString release];
			[string release];
		#endif
			i0=i+1;
		}
		if(0==str[i])
		{
			break;
		}
		i++;
	}

	if(0==*wid || 0==*hei)
	{
		*dstBytePerLine=0;
		return NULL;
	}

	*wid=(*wid+3)&~3;

	unsigned char *retBuf=NULL;
	NSBitmapImageRep *bmpImg=[[NSBitmapImageRep alloc]
								 initWithBitmapDataPlanes:NULL
								 pixelsWide:*wid
								 pixelsHigh:*hei
								 bitsPerSample:8
								 samplesPerPixel:3
								 hasAlpha:NO
								 isPlanar:NO
								 colorSpaceName:NSDeviceRGBColorSpace
								 bytesPerRow:0
								 bitsPerPixel:32];
	if(NULL!=bmpImg)
	{
		switch(dstBitPerPixel)
		{
		case 1:
			*dstBytePerLine=((*wid+31)&~31)/8;
			break;
		case 16:
		case 32:
			*dstBytePerLine=(*wid)*dstBitPerPixel/8;
			break;
		}

		retBuf=YsMacOSXSystemFont_AllocBuffer((*dstBytePerLine)*(*hei));
		if(NULL!=retBuf)
		{
			for(i=0; i<(*dstBytePerLine)*(*hei); i++)
			{
				retBuf[i]=0;
			}

			NSGraphicsContext *gc=[NSGraphicsContext graphicsContextWithBitmapImageRep:bmpImg];
			NSGraphicsContext *prevGc=[NSGraphicsContext currentContext];
			[NSGraphicsContext setCurrentContext:gc];

			[gc setShouldAntialias:YES];
			[[NSColor blackColor] set];

			NSRect rect;
			rect.origin.x=0;
			rect.origin.y=0;
			rect.size.width=*wid;
			rect.size.height=*hei;
			NSRectFill(rect);

			[[NSColor whiteColor] set];
			i=0;
			i0=0;
			rect.origin.y=*hei-1;
			for(;;)
			{
				if(0==str[i] || '\n'==str[i])
				{
					NSString *rawString=[[NSString alloc]
								initWithBytes:str+i0
								length:(i-i0)*sizeof(wchar_t)
								encoding:encoding];
					NSAttributedString *string=[[NSAttributedString alloc]
								initWithString:rawString attributes:ctxt->attrib];
					
					rect.size=[string size];
					rect.origin.y-=rect.size.height;
					[string drawWithRect:rect options:NSStringDrawingUsesLineFragmentOrigin];
					
				#if !__has_feature(objc_arc)
					[rawString release];
					[string release];
				#endif
					i0=i+1;
				}
				if(0==str[i])
				{
					break;
				}
				i++;
			}

			unsigned char *srcDat=[bmpImg bitmapData];
			unsigned int srcBytePerLine=[bmpImg bytesPerRow];
			int y;
			for(y=0; y<*hei; y++)
			{
				int yInBmp=(0==reverse ? y : *hei-1-y);
				unsigned char *srcLineTop=srcDat+yInBmp*srcBytePerLine;
				unsigned char *dstLineTop=retBuf+y*(*dstBytePerLine);

				unsigned char *dstPtr=dstLineTop;
				unsigned char dstMask=0x80;

				int x;
				for(x=0; x<*wid; x++)
				{
					const unsigned char *col;
					unsigned char alpha=srcLineTop[x*4];
					if(2<srcLineTop[x*4])
					{
						col=fgCol;
					}
					else
					{
						col=bgCol;
					}
					
					switch(dstBitPerPixel)
					{
					case 1:
						if(2<alpha)
						{
							(*dstPtr)|=dstMask;
						}
						
						if(1==dstMask)
						{
							dstMask=0x80;
							dstPtr++;
						}
						else
						{
							dstMask>>=1;
						}
						break;
					case 16:
						dstLineTop[x*2  ]=col[0];
						dstLineTop[x*2+1]=alpha;
						break;
					case 32:
						dstLineTop[x*4  ]=col[0];
						dstLineTop[x*4+1]=col[1];
						dstLineTop[x*4+2]=col[2];
						dstLineTop[x*4+3]=alpha;
						break;
					}
				}
			}
			[NSGraphicsContext setCurrentContext:prevGc];
		}
	#if !__has_feature(objc_arc)
		[bmpImg release];
	#endif
	}

	if(NULL==retBuf)
	{
		*wid=0;
		*hei=0;
	}

	return retBuf;
}

void YsMacOSXSystemFont_GetTightBitmapSize(void *ctxtPtr,int *wid,int *hei,const wchar_t str[])
{
	if(NULL==str)
	{
		*wid=0;
		*hei=0;
		return;
	}

	YsMacOSXSystemFontContext *ctxt=(YsMacOSXSystemFontContext *)ctxtPtr;

	wchar_t tst=0x0001;
	unsigned char *tstByte=(unsigned char *)&tst;
	NSStringEncoding encoding;

	if(0!=tstByte[0])
	{
		encoding=NSUTF16LittleEndianStringEncoding;
	}
	else
	{
		encoding=NSUTF16BigEndianStringEncoding;
	}

	*wid=0;
	*hei=0;

	int i=0,i0=0;
	for(;;)
	{
		if(0==str[i] || '\n'==str[i])
		{
			NSString *rawString=[[NSString alloc]
									initWithBytes:str+i0 
									length:(i-i0)*sizeof(wchar_t) 
									encoding:encoding];

			NSAttributedString *string=[[NSAttributedString alloc]
								 initWithString:rawString attributes:ctxt->attrib];

			NSSize strSize=[string size];
			*wid=(*wid<strSize.width ? strSize.width : *wid);
			*hei+=strSize.height;

		#if !__has_feature(objc_arc)
			[rawString release];
			[string release];
		#endif
			i0=i+1;
		}
		if(0==str[i])
		{
			break;
		}
		i++;
	}

	if(0==*wid || 0==*hei)
	{
		*wid=0;
		*hei=0;
	}
}
