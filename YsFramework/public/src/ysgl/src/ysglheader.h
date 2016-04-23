/* ////////////////////////////////////////////////////////////

File Name: ysglheader.h
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

#ifndef YSGLHEADER_IS_INCLUDED
#define YSGLHEADER_IS_INCLUDED
/* { */

/* Force Visual C++ to type-mismatching error. */
#pragma warning( error : 4028)
#pragma warning( error : 4047)

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		// Prevent inclusion of winsock.h
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		#undef WIN32_LEAN_AND_MEAN
	#else
		// Too late.  Just include it.
		#include <windows.h>
	#endif
#elif !defined GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
#endif

// I didn't find a convenient macro to identify iOS.  Please add a macro __APPLE_IOS__ to the preprocessor macro option.

#ifndef __APPLE__
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glext.h>
#else
    #include <TargetConditionals.h>
    #if TARGET_OS_EMBEDDED!=0 || TARGET_OS_IPHONE!=0 || TARGET_OS_IPHONE_SIMULATOR!=0
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>
		typedef double GLdouble;
	#else
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <OpenGL/glext.h>
	#endif
#endif

#if !defined(GL_PROGRAM_POINT_SIZE) && defined(GL_PROGRAM_POINT_SIZE_EXT)
#define GL_PROGRAM_POINT_SIZE GL_PROGRAM_POINT_SIZE_EXT
#endif

#if !defined(GL_CLAMP) && defined(GL_CLAMP_TO_EDGE)
#define GL_CLAMP GL_CLAMP_TO_EDGE
#endif


#ifdef _WIN32
	#include "ysglwindows.h" // Hey Microsoft, can you concede and add support for higher versions of OpenGL in Visual C++ ?  Thank you!  Nobody wants Direct 3D b**ls**t!
#endif

/* } */
#endif
