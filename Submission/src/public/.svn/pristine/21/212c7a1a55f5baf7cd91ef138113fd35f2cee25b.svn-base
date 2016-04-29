PROJECTNAME=yssystemfont
SRC_ROOT=..\..\..
IS_LIBRARY_PROJECT=1
DEPEND_ON=
OWN_HEADER_PATH=..



# {
# BEGIN YS Standard NMAKE MAKEFILE HEADER 20140801 >>
#
# Variable PROJECTNAME must be defined before the header.
# Variable IS_LIBRARY_PROJECT must be set 0 or 1
#

!IFNDEF IS_LIBRARY_PROJECT
!ERROR IS_LIBRARY_PROJECT not defined.
!ENDIF

!IFNDEF PROJECTNAME
!ERROR PROJECTNAME not defined.
!ENDIF

!IFNDEF SRC_ROOT
!ERROR SRC_ROOT not defined.
!ENDIF

!IFNDEF LINKSUBSYS
LINKSUBSYS=CONSOLE
!ENDIF

CC=cl
INCOPT=/I
LIBPREFIX=
LIBEXT=.lib
RM=del
MD=mkdir
CP=copy
LIBEXE=lib.exe
MAKEEXE=nmake -nologo
C_OPTION_AVAILABLE=0
SLASH=\
CHAIN=&

!IFNDEF BITNESSARCH

BITNESSUTIL=ys_temp_util_sizeofpointer
BITNESSUTILCPP=$(BITNESSUTIL).cpp
BITNESSUTILEXE=$(BITNESSUTIL).exe

!IF [@echo int main(void){return sizeof(void *);} > $(BITNESSUTILCPP)]==0
!MESSAGE Created $(BITNESSUTILCPP)
!ELSE
!ERROR Cannot create $(BITNESSUTILCPP)
!ENDIF

!IF [@cl.exe /nologo $(BITNESSUTILCPP)]==0
!MESSAGE Compiled $(BITNESSUTILCPP)
!ELSE
!ERROR Cannot compile bitness identifier!
!ENDIF

!IF [$(BITNESSUTILEXE)]==4
!MESSAGE Detected 32-bit compiler
BITNESS=32
BITNESSARCH=win32
!ELSEIF [$(BITNESSUTILEXE)]==8
!MESSAGE Detected 64-bit compiler
BITNESS=64
BITNESSARCH=win64
!ELSE
!ERROR Cannot identify bitness
!ENDIF

!IF [del $(BITNESSUTIL).*]==0
!ENDIF

!ENDIF
#Closing IFNDEF BITNESSARCH



!IF "$(BITNESSARCH)"=="win32"
SYSTEMID=5.01
LINKMACHINE=/MACHINE:X86
!ELSEIF "$(BITNESSARCH)"=="win64"
SYSTEMID=5.02
LINKMACHINE=/MACHINE:x64
!ELSE
!ERROR Unknown bitness-architecture.
!ENDIF



!IFDEF USERLIBDIR
#!MESSAGE User Library Directory=$(USERLIBDIR)
!ELSE
!MESSAGE Environment variable USERLIBDIR is not defined.  Default to C:\lib
USERLIBDIR=C:\lib
!ENDIF

!IFDEF USERINCDIR
#!MESSAGE User Include Directory=$(USERINCDIR)
!ELSE
!MESSAGE Environment variable USERINCDIR is not defined.  Default to C:\include
USERINCDIR=C:\include
!ENDIF



!IFDEF USEROBJDIR
#!MESSAGE User Object Directory=$(USEROBJDIR)
!ELSE
!MESSAGE Environment variable USEROBJDIR is not defined.  Default to C:\tmp\obj
USEROBJDIR=C:\tmp\obj
!ENDIF



OBJDIR=$(USEROBJDIR)\$(BITNESSARCH)\$(PROJECTNAME)
LIBDIR=$(USERLIBDIR)\$(BITNESSARCH)
BINDIR=.\bin\$(BITNESSARCH)
PDBFILE=$(OBJDIR)\$(PROJECTNAME).pdb
PCHFILE=$(OBJDIR)\$(PROJECTNAME).pch
#
# END YS Standard NMAKE MAKEFILE HEADER <<
# }



TARGET="$(LIBDIR)\$(PROJECTNAME).lib"



.SUFFIXES: .c .cpp .obj

LIBFLAGS = /NOLOGO

ADDITIONAL_INCLUDE_PATH=

ADDITIONAL_LIBRARIES=

CFLAGS=/nologo /c /W4 /Zi /DWIN32 /O2 /MT /Fd$(PDBFILE) /Fp$(PCHFILE) /wd4996  /I.. $(ADDITIONAL_INCLUDE_PATH)



HEADERS=..\yssystemfont.h

OBJS=$(OBJDIR)\yswin32systemfont.obj



all : dependencies $(TARGET)

targetonly : $(TARGET)

$(TARGET) : $(HEADERS) $(OBJDIR) $(LIBDIR) $(OBJS)
		$(LIBEXE) @<<
	$(LIBFLAGS) /OUT:"$(TARGET)" $(OBJS)
<<

dependencies :

clean_dependencies :

clean_self :
	$(RM) $(OBJS)
	$(RM) $(TARGET)
	$(RM) *.pdb

$(OBJDIR) :
	md $(OBJDIR)

$(LIBDIR) :
	md $(LIBDIR)


inst :
	$(CP) $(TARGET) $(LIBDIR)

cln : clean_dependencies clean_self

rebuild: cln $(TARGET)





$(OBJDIR)\yswin32systemfont.obj : yswin32systemfont.cpp
	$(CC) $? $(CFLAGS) /Fo$@


