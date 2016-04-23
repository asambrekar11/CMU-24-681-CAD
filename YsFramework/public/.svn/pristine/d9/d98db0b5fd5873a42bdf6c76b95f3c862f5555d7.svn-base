#ifndef YSPLATFORMIDENT_IS_INCLUDED
#define YSPLATFORMIDENT_IS_INCLUDED
/* { */

enum YS_PLATFORM_IDENT
{
	YS_PLATFORM_IDENT_UNKNOWN,
	YS_PLATFORM_IDENT_WINDOWS,
	YS_PLATFORM_IDENT_MACOSX,
	YS_PLATFORM_IDENT_IOS,
	YS_PLATFORM_IDENT_LINUX
};



#ifdef _WIN32
	#define YS_PLATFORM_IS_WINDOWS
#elif defined(__linux) || defined(linux) || defined(LINUX)
	#define YS_PLATFORM_IS_LINUX
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_EMBEDDED!=0 || TARGET_OS_IPHONE!=0 || TARGET_OS_IPHONE_SIMULATOR!=0
		#define YS_PLATFORM_IS_IOS
	#else
		#define YS_PLATFORM_IS_MACOSX
	#endif
#endif



inline YS_PLATFORM_IDENT YsIdentifyPlatform(void)
{
	#if   defined(YS_PLATFORM_IS_MACOSX)
		return YS_PLATFORM_IDENT_MACOSX;
	#elif defined(YS_PLATFORM_IS_IOS)
		return YS_PLATFORM_IDENT_IOS;
	#elif defined(YS_PLATFORM_IS_WINDOWS)
		return YS_PLATFORM_IDENT_WINDOWS;
	#elif defined(YS_PLATFORM_IS_LINUX)
		return YS_PLATFORM_IDENT_LINUX;
	#else
		return YS_PLATFORM_IDENT_UNKNOWN;
	#endif
}



/* } */
#endif
