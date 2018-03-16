#ifndef __EasyCrossPlatform_APPRuntimeFile__
	#define __EasyCrossPlatform_APPRuntimeFile__
	#include <EasyCP_Common.h>
	#include <whereami/whereami.h>
	namespace EasyCrossPlatform {
		namespace Runtime {
			namespace Path {
				std::string APPPath(); //Returns the directory that the program is in, ending with "\\" or "/"
				std::string APPExeName();
			}
		}
	}
#endif