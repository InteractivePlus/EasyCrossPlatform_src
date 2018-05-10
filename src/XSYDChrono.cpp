#include <XSYDChrono.h>

double EasyCrossPlatform::Runtime::Chrono::systemTime()
{
	std::chrono::time_point<std::chrono::system_clock> nowTime = std::chrono::system_clock::now();
	auto nowDuration = nowTime.time_since_epoch();
	double myNowTime = ((double)nowDuration.count()) / 10000000.00;
	return myNowTime;
}

double EasyCrossPlatform::Runtime::Chrono::accurateTimer()
{
	auto nowTime = std::chrono::steady_clock::now();
	auto nowDuration = nowTime.time_since_epoch();
	double myNowTime = ((double)nowDuration.count()) / 1000000000.00;
	return myNowTime;
}

double EasyCrossPlatform::Runtime::Chrono::toDoubleTime(std::time_t Time)
{
	return static_cast<double>(Time);
}

std::tm EasyCrossPlatform::Runtime::Chrono::ToLocalTimeStruct(double TimeInUnixStamp)
{
	std::time_t mTime_T = static_cast<std::time_t>(TimeInUnixStamp);
	std::tm* mTMStruct = std::localtime(&mTime_T);
	return (*mTMStruct);
}

std::tm EasyCrossPlatform::Runtime::Chrono::ToGMTTimeStruct(double TimeInUnixStamp)
{
	std::time_t mTime_T = static_cast<std::time_t>(TimeInUnixStamp);
	std::tm* mTMStruct = std::gmtime(&mTime_T);
	
	return (*mTMStruct);
}

double EasyCrossPlatform::Runtime::Chrono::fromTimeStruct(std::tm TimeStruct, bool isGMTTime)
{
	std::time_t mTime_T = std::mktime(&TimeStruct);
	double mDoubleTime = static_cast<double>(mTime_T);
	if (isGMTTime) {
		mDoubleTime += Chrono::getTimeZone() * (3600);
	}
	return mDoubleTime;
}

int EasyCrossPlatform::Runtime::Chrono::getTimeZone()
{
	double mTime = Chrono::systemTime();
	std::tm mGMTTm = Chrono::ToGMTTimeStruct(mTime);
	std::tm mLocalTm = Chrono::ToLocalTimeStruct(mTime);
	int time_zone = mLocalTm.tm_hour - mLocalTm.tm_hour;
	if (time_zone < -12) {
		time_zone += 24;
	}
	else if (time_zone > 12) {
		time_zone -= 24;
	}
	return time_zone;
}

void EasyCrossPlatform::Runtime::Chrono::sleepFor(double Second)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(Second));
}
