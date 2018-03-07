#ifndef __FILE_CROSSPLATFORM_OSDETECTION_INCLUDED__
	#define __FILE_CROSSPLATFORM_OSDETECTION_INCLUDED__
	//**********************************************OSDetection**********************************************
	//CROSSPLATFORM_OS_TYPE will be deinfed. It is going to be either CROSSPLATFORM_OS_WIN or CROSSPLATFORM_OS_UNIX
	//CROSSPLATFORM_OS_IS_WIN, EASYCROSSPLATFORM_PLATFORM_WINDOWS will be defined if running windows.
	//CROSSPLATFORM_OS_IS_UNIX, EASYCROSSPLATFORM_PLATFORM_UNIX will be defined if running UNIX.
	//CROSSPLATFORM_OS_STRING will be defined to "windows", "linux", "macosx"
	
	#define CROSSPLATFORM_OS_WIN 0
	#define CROSSPLATFORM_OS_UNIX 1
	#define CROSSPLATFORM_OS_MAC 2
	#if defined(__WINDOWS_) || defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
		#define CROSSPLATFORM_OS_TYPE CROSSPLATFORM_OS_WIN
		#define CROSSPLATFORM_OS_IS_WIN 1
		#define EASYCROSSPLATFORM_PLATFORM_WINDOWS 1
		#define CROSSPLATFORM_OS_STRING "windows"
	#elif defined(linux) || defined(__LINUX__) || defined(__linux) || defined(__linux) || defined(_UNIX) || defined(__unix__) || defined(__GNU__) || defined(__GLIBC__) || defined(_POSIX_VERSION)
		#define CROSSPLATFORM_OS_TYPE CROSSPLATFORM_OS_UNIX
		#define CROSSPLATFORM_OS_IS_UNIX 1
		#define EASYCROSSPLATFORM_PLATFORM_UNIX 1
		#define CROSSPLATFORM_OS_STRING "macosx"
	#elif defined(__APPLE__) || defined(APPLE)
		#define CROSSPLATFORM_OS_TYPE CROSSPLATFORM_OS_MAC
		#define CROSSPLATFORM_OS_IS_MAC 1
		#define EASYCROSSPLATFORM_PLATFORM_UNIX 1
		#define CROSSPLATFORM_OS_STRING "macosx"
	#endif
	//*******************************************************************************************************
	//**********************************************ArchDetection********************************************
	//EASYCROSSPLATFORM_ARCHITECTURE_X86, EASYCROSSPLATFORM_ARCHITECTURE_X64, EASYCROSSPLATFORM_ARCHITECTURE_ARM64, EASYCROSSPLATFORM_ARCHITECTURE_ARMv5, EASYCROSSPLATFORM_ARCHITECTURE_ARMv6, EASYCROSSPLATFORM_ARCHITECTURE_ARMv7, EASYCROSSPLATFORM_ARCHITECTURE_ARMv8 will be defined when compiling
	//EASYCROSSPLATFORM_ARCH will be set to either "x86","x64","arm64","armv5","armv6","armv7","armv8"
	#if defined(EASYCROSSPLATFORM_ARCHITECTURE_X86)
		#define EASYCROSSPLATFORM_ARCH "x86"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_X64)
		#define EASYCROSSPLATFORM_ARCH "x64"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARM64)
		#define EASYCROSSPLATFORM_ARCH "arm64"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARMv5)
		#define EASYCROSSPLATFORM_ARCH "armv5"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARMv6)
		#define EASYCROSSPLATFORM_ARCH "armv6"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARMv7)
		#define EASYCROSSPLATFORM_ARCH "armv7"
	#elif defined(EASYCROSSPLATFORM_ARCHITECTURE_ARMv8)
		#define EASYCROSSPLATFORM_ARCH "armv8"
	#endif
	//*******************************************************************************************************
#endif