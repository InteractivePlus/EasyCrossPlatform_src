#ifndef __EasyCrossPlatform_ChronoFile__
	#define __EasyCrossPlatform_ChronoFile__
	#include <chrono>
	#include <EasyCP_Common.h>
	namespace EasyCrossPlatform {
		namespace Chrono {
			double systemTime();
			double accurateTime();
		}
	}
#endif