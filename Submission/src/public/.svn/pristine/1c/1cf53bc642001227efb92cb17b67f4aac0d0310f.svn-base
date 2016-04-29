/* ////////////////////////////////////////////////////////////

File Name: ysspecialpath.h
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

#ifndef YSSPECIALPATH_IS_INCLUDED
#define YSSPECIALPATH_IS_INCLUDED
/* { */

#include <ysclass.h>


// Examples:
// [UserDir]
//   Windows C:\Users\soji
//   Linux   
//   MacOSX  
// [UserDocDir]
//   Windows C:\Users\soji\My Documents
//   Linux   ~/Documents
//   MacOSX  
// [UserAppDataDir]
//   Windows C:\Users\soji\Application Data
//   Linux   ~/ApplicationData (I made up)
//   MacOSX  ~/Library/Application Support
// [AllUsersDir]
//   Windows C:\Users\All Users\Application Data
//   Linux   No such thing (returns YSERR)
//   MacOSX  Does it exist?

// GetProgramBaseDir will return the directory where the executable files are stored in Windows and Linux,
// the bundle directory in MacOSX.

class YsSpecialPath
{
	enum
	{
		MaxPath=512
	};

public:
	static YSRESULT GetProgramFileName(YsString &path);
	static YSRESULT GetProgramBaseDir(YsString &path);
	static YSRESULT GetUserDir(YsString &path);
	static YSRESULT GetUserDocDir(YsString &path);
	static YSRESULT GetUserAppDataDir(YsString &path);
	static YSRESULT GetAllUsersDir(YsString &path);

	static YSRESULT GetProgramFileName(YsWString &wpath);
	static YSRESULT GetProgramBaseDir(YsWString &wpath);
	static YSRESULT GetUserDir(YsWString &wpath);
	static YSRESULT GetUserDocDir(YsWString &wpath);
	static YSRESULT GetUserAppDataDir(YsWString &wpath);
	static YSRESULT GetAllUsersDir(YsWString &wpath);

	static YSRESULT GetProgramFileNameUTF8(YsString &path);
	static YSRESULT GetProgramBaseDirUTF8(YsString &path);
	static YSRESULT GetUserDirUTF8(YsString &path);
	static YSRESULT GetUserDocDirUTF8(YsString &path);
	static YSRESULT GetUserAppDataDirUTF8(YsString &path);
	static YSRESULT GetAllUsersDirUTF8(YsString &path);
};


/* } */
#endif
