#ifndef __EasyCrossPlatform_APPRuntimeFile__
	#define __EasyCrossPlatform_APPRuntimeFile__
	#include <EasyCP_Common.h>
	#include <whereami/whereami.h>
	#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
		#include <direct.h>  
	#else //if defined(EASYCROSSPLATFORM_PLATFORM_UNIX)
		#include <unistd.h>  
	#endif
	namespace EasyCrossPlatform {
		namespace Runtime {
			namespace Path {
				const constexpr unsigned int PathMaxSize = 1024U;
				std::string APPPath(); //Returns the directory that the program is in, ending with "\\" or "/"
				std::string APPExeName();
				std::string CWDPath();
			}
		}
	}
#endif