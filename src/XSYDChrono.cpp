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

double EasyCrossPlatform::Runtime::Chrono::fromTimeStruct(std::tm TimeStruct)
{
	std::time_t mTime_T = std::mktime(&TimeStruct);
	double mDoubleTime = static_cast<double>(mTime_T);
	return mDoubleTime;
}

void EasyCrossPlatform::Runtime::Chrono::sleepFor(double Second)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(Second));
}
