#ifndef __EASYCROSSPLATFORM_FILE_COMMONFILE_INCLUDED__
	#define __EASYCROSSPLATFORM_FILE_COMMONFILE_INCLUDED__
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS  //Make sure VS compiles this file correctly.
	#endif

	#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 0 //Make sure CrytoPP compiles correctly
	#define NULL 0 //NULL can be interpreted using 0
	#include <stdint.h>	
	typedef uint8_t byte; //Byte is unsigned char, you can cast it just from char

	#define EASYCROSSPLATFORM_VERSION 1;
	#include <EasyCrossPlatform/Internal/CrossPlatform_OSDetection.h>
	//***************************************
	#include <stdio.h>
	#include <algorithm>
	#include <memory>
	#include <string>
	#if defined(EASYCROSSPLATFORM_PLATFORM_POSIX)
		#include <string.h>
	#endif
	
	#include <assert.h>
	#include <cstddef> //nullptr
	#include <cassert> //Debug
	#include <exception> //Exception
	#include <stdexcept> //StdException
	#include <vector> //STD VECTOR
	#include <deque> //STD Deque
	#include <list> //STD List
	#include <map> //STD Dictionary
	#include <unordered_map> //STD Salt-Table Dictionary
	#include <utility> //Utilities, now using pairs in them.
	#include <stdlib.h> //StdLib
	#include <cmath> //std::pow, std::round, etc.
#endif
