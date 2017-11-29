#ifndef __XSYDCommonHeaderFile__
	#define __XSYDCommonHeadersFile__
	#define NULL 0
	//Define系统类型
	#define EASYCROSSPLATFORM_VERSION 0;
	/* 
	#if defined(linux) || defined(_UNIX) || defined(__LINUX__) || defined(__unix__)
		#define EASYCROSSPLATFORM_PLATFORM_LINUX true
	#elif defined(__WINDOWS_) || defined(_WIN32)
		#define EASYCROSSPLATFORM_PLATFORM_WINDOWS true
	#endif
	*/
	#if defined(__WINDOWS_) || defined(_WIN32)
        #define EASYCROSSPLATFORM_PLATFORM_WINDOWS
    #else
        #define EASYCROSSPLATFORM_PLATFORM_LINUX
    #endif
	#include <string> //标准字符串
	#include <chrono> //时间
	#include <cassert> //Debug类
	#include <exception> //Exception异常类
	#include <stdexcept> //StdException通用异常类
	#include <vector> //STD动态数组Vector
	#include <deque> //STD队列[双向数组]Deque
	#include <list> //STD列表
#endif
