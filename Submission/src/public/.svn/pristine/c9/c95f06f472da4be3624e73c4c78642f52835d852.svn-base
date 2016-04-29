PROJECTNAME=yssystemfont
SRC_ROOT=../../..
IS_LIBRARY_PROJECT=1
OWN_HEADER_PATH=..
DEPEND_ON=



# {
# YS Standard Makefile Header for Unix and MacOSX 20140801 >>
#
# Variable PROJECTNAME and SRC_ROOT must be defined before the header.
#

# Self-memo: $* $> $@ etc are officially (?) called 'automatic variables'
# Self-memo: $(lastword $(MAKEFILE_LIST)) will give the current makefile name


# Identifying compiler >>
# Priority is given to clang over g++
TESTCLANG:=$(shell which clang 1> /dev/null; echo $$?)
TESTGPP:=$(shell which g++ 1> /dev/null; echo $$?)

ifeq ($(TESTCLANG),0)
CC=clang
else ifeq ($(TESTGPP),0)
CC=g++
else
$(error C++ Compiler not found.)
endif
# Identifying compiler >>


RM=rm
MD=mkdir
CP=cp
INCOPT=-I
LIBPREFIX=lib
LIBEXT=.a
CPPFLAGS=-std=c++0x
MAKEEXE=make
C_OPTION_AVAILABLE=1
SLASH=/
CHAIN=;

BITNESSTESTSRC=ysbitnesstest.cpp
BITNESSTESTEXE=ysbitnesstest

ifndef PROJECTNAME
$(error Variable PROJECTNAME not defined.)
endif
ifndef SRC_ROOT
$(error Variable SRC_ROOT not defined.)
endif
ifndef IS_LIBRARY_PROJECT
$(error Variable IS_LIBRARY_PROJECT not defined.)
endif


ifndef YS_BITNESS
GENSRC:=$(shell echo 'int main(void){printf("%d",8*(int)sizeof(void *));return 0;} ' 1> $(BITNESSTESTSRC))
BITNESSCOMPILE:=$(shell g++ $(BITNESSTESTSRC) -o $(BITNESSTESTEXE) -include stdio.h)
SIZEOFPOINTER:=$(shell ./$(BITNESSTESTEXE))
YS_BITNESS:=$(SIZEOFPOINTER)
CLEANTEMPORARYCPP:=$(shell rm $(BITNESSTESTSRC) $(BITNESSTESTEXE))
endif

ifeq ($(YS_BITNESS),32)
  CBITNESSFLAG=-m32
else
  ifeq ($(YS_BITNESS),64)
    CBITNESSFLAG=-m64
  endif
endif

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
PLATFORM=linux
PLATFORMSHORT=unx
PLATFORMLIB=$(X11LIB) -lGLU -lGL
APPDIR=.
APPDIRSIGNATURE=
APPDIRSIGNATURERULE=dummyRule
OBJSIMPLEWIN=$(OBJDIR)/fsglxwrapper.o $(OBJDIR)/fsglxkeymap.o
endif

ifeq ($(UNAME), Darwin)
PLATFORM=macosx
PLATFORMSHORT=mac
PLATFORMLIB=-framework Cocoa -framework OpenGL
APPDIR=$(PROJECTNAME).app/Contents/MacOS
APPDIRSIGNATURE=$(APPDIR)/signature
APPDIRSIGNATURERULE=$(APPDIRSIGNATURE)
OBJSIMPLEWIN=$(OBJDIR)/fsmacosxwrapperobjc.o $(OBJDIR)/fsmacosxwrappercpp.o
ifeq ($(IS_LIBRARY_PROJECT),0)
MAKEBUNDLEDIR:=$(shell mkdir -p $(APPDIR))
endif
endif

ifndef YSBIN_ROOT
YSBIN_ROOT=$(HOME)/ysbin
endif

YSOBJ_ROOT=$(YSBIN_ROOT)/$(PLATFORM)$(YS_BITNESS)/obj
USERLIBDIR=$(YSBIN_ROOT)/$(PLATFORM)$(YS_BITNESS)/lib

OBJDIR=$(YSOBJ_ROOT)/$(PROJECTNAME)
LIBDIR=$(USERLIBDIR)
BINDIR=./bin/$(PLATFORM)$(YS_BITNESS)
MAKEFILE=$(lastword $(MAKEFILE_LIST))
#
# YS Standard Makefile Header for Unix and MacOSX <<
# }



YSSYSTEMFONTDIR=..

TARGET=$(LIBDIR)/lib$(PROJECTNAME).a

ADDITIONAL_INCLUDE_PATH=

ADDITIONAL_LIBRARIES=

CC = g++
CPP= g++
CFLAGS = -c -O3 $(CBITNESSFLAG) -fopenmp -I$(YSSYSTEMFONTDIR)
#-Wall gives a warning when member variable may not be initialized.
#-m32 to force 32-bit executable

# .IGNORE:




.SUFFIXES: .c .cpp .obj




HEADERS=../yssystemfont.h

OBJS=$(OBJDIR)/ysunixsystemfont.o


all : dependencies $(TARGET)

targetonly : $(TARGET)

dependencies :

clean_dependencies :

clean_self :
	rm -f $(OBJS)
	rm -f $(TARGET)

$(TARGET) : $(OBJDIR)/signature $(LIBDIR)/signature $(HEADERS) $(OBJS)
	ar cru $(TARGET) $(OBJS)
	ranlib $(TARGET)

rebuild : cln $(TARGET)

cln : clean_dependencies clean_self

$(OBJDIR)/signature :
	mkdir -p $(OBJDIR)
	echo signature > $@

$(LIBDIR)/signature :
	mkdir -p $(LIBDIR)
	echo signature > $@

#.c.o:
#	$(CC) $(CFLAGS) $*.c
#
#.cpp.o:
#	$(CPP) $(CFLAGS) $*.cpp
#

$(OBJDIR)/ysunixsystemfont.o : ysunixsystemfont.cpp
	$(CC) $? $(CFLAGS) -o $@


