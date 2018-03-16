#ifndef __EasyCrossPlatform_ChronoFile__
	#define __EasyCrossPlatform_ChronoFile__
	#include <EasyCP_Common.h>
	#include <chrono>
	#include <ctime>
	namespace EasyCrossPlatform {
		namespace Runtime {
			namespace Chrono {
				double systemTime();
				double accurateTimer();
				std::tm ToLocalTimeStruct(double TimeInUnixStamp); //Converting from double to tm struct will lose percision.
				std::tm ToGMTTimeStruct(double TimeInUnixStamp); //Converting from double to tm struct will lose percision.
				double fromTimeStruct(std::tm TimeStruct);
			}
		}
	}
#endif