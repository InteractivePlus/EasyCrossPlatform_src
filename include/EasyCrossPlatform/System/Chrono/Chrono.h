#ifndef __EASYCROSSPLATFORM_FILE_CHRONOH_INCLUDED__
    #define __EASYCROSSPLATFORM_FILE_CHRONOH_INCLUDED__
    #include <EasyCrossPlatform/Internal/EasyCP_Common.h>
    #include <chrono>
	#include <ctime>
    
    namespace EasyCrossPlatform{
        namespace System{
            namespace Chrono{
                    //Remember that GMT and UTC are the same TIME!
                    double systemTime();
                    double accurateTimer();
                    double toDoubleTime(std::time_t Time);
                    std::tm ToLocalTimeStruct(double TimeInUnixStamp); //Converting from double to tm struct will lose percision.
                    std::tm ToGMTTimeStruct(double TimeInUnixStamp); //Converting from double to tm struct will lose percision.
                    double fromTimeStruct(std::tm TimeStruct, bool isGMTTime = false);
                    int getTimeZone(); //timeZone is the difference of (LocalTimeHr - UTCTimeHr)
                    void sleepFor(double Second);
                    void sleepForMilli(unsigned int MilliSecond);
            }
        }
    }
#endif