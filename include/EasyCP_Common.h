#ifndef __XSYDCommonHeaderFile__
	#define __XSYDCommonHeadersFile__
	#define NULL 0 //NULL can be interpreted using 0
	typedef unsigned char byte; //Byte is unsigned char, you can cast it just from char
	//Define系统类型
	#define EASYCROSSPLATFORM_VERSION 0;
	//Linux Symbol linux, _UNIX, __LINUX__, __unix__
	//Remember that EASYCROSSPLATFORM_ARCHITECTURE_X86 / EASYCROSSPLATFORM_ARCHITECTURE_X64 / EASYCROSSPLATFORM_ARCHITECTURE_ARM64 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv5 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv6 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv7 / EASYCROSSPLATFORM_ARCHITECTURE_ARMv8 will be defined in the compile process.
	//If an ARM CPU is found, EASYCROSSPLATFORM_ARCHITECTURE_ARM will be defined as well.
	//****CROSSPLATFORM TYPE DEFINITION****
	#define EASYCROSSPLATFORM_PLATFORMTYPE_UNKNOWN 0
	#define EASYCROSSPLATFORM_PLATFORMTYPE_IOSSIMULATOR 1
	#define EASYCROSSPLATFORM_PLATFORMTYPE_IOS 2
	#define EASYCROSSPLATFORM_PLATFORMTYPE_MAC 3
	#define EASYCROSSPLATFORM_PLATFORMTYPE_ANDROID 4
	#define EASYCROSSPLATFORM_PLATFORMTYPE_LINUX 5
	#define EASYCROSSPLATFORM_PLATFORMTYPE_UNIX 6
	#define EASYCROSSPLATFORM_PLATFORMTYPE_OTHERPOSIX 7
	#define EASYCROSSPLATFORM_PLATFORMTYPE_WINDOWS 8
	//**************************************
	
	//****CROSSPLATFORM_MAJOR_TYPE_RECOGNITION****
	#if defined(__WINDOWS_) || defined(_WIN32) || defined(_WIN64)
        #define EASYCROSSPLATFORM_PLATFORM_WINDOWS 1
	#else
        #define EASYCROSSPLATFORM_PLATFORM_UNIX 1
    #endif
	//********************************************
	
	//****CROSSPLATFORM_TYPE_RECOGNITION****
	#if defined(__WINDOWS_) || defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
		#define EASYCROSSPLATFORM_PLATFORM_IS_WINDOWS 1
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_WINDOWS
	#elif defined(__APPLE__) || defined(APPLE)
		#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR
			#define EASYCROSSPLATFORM_PLATFORM_IS_IOSSIMULATOR 1
			#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_IOSSIMULATOR
		#elif TARGET_OS_IOS //TARGET_OS_IPHONE can also be used
			#define EASYCROSSPLATFORM_PLATFORM_IS_IOS 1
			#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_IOS
		#elif TARGET_OS_MAC
			#define EASYCROSSPLATFORM_PLATFORM_IS_MAC 1
			#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_MAC
		#endif
	#elif defined(__ANDROID__)
		#define EASYCROSSPLATFORM_PLATFORM_IS_ANDROID 1
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_ANDROID
	#elif defined(linux) || defined(__LINUX__) || defined(__linux) || defined(__linux)
		#define EASYCROSSPLATFORM_PLATFORM_IS_LINUX 1
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_LINUX
	#elif defined(_UNIX) || defined(__unix__) || defined(__GNU__) || defined(__GLIBC__)
		#define EASYCROSSPLATFORM_PLATFORM_IS_UNIX 1
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_UNIX
	#elif defined(_POSIX_VERSION)
		#define EASYCROSSPLATFORM_PLATFORM_IS_OTHERPOSIX 1
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_OTHERPOSIX
	#else
		#define EASYCROSSPLATFORM_PLATFORMTYPE EASYCROSSPLATFORM_PLATFORMTYPE_UNKNOWN
	#endif
	//***************************************
	#include <stdio.h>
	#include <memory>
	#include <string> //标准字符串
	#if defined(EASYCROSSPLATFORM_PLATFORM_UNIX)
		#include <string.h>
	#endif
	#include <chrono> //时间
	#include <assert.h>
	#include <cassert> //Debug类
	#include <exception> //Exception异常类
	#include <stdexcept> //StdException通用异常类
	#include <vector> //STD动态数组Vector
	#include <deque> //STD队列[双向数组]Deque
	#include <list> //STD列表
	#include <map> //STD Dictionary
	#include <utility> //Utilities, now using pairs in them.
	#include <stdlib.h> //StdLib
#endif
