#ifndef __XSYDCommonHeaderFile__
	#define __XSYDCommonHeadersFile__
	#define NULL 0 //NULL can be interpreted using 0
	typedef unsigned char byte; //Byte is unsigned char, you can cast it just from char
	//Define系统类型
	#define EASYCROSSPLATFORM_VERSION 1;
	#include <CrossPlatform_OSDetection.h>
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
