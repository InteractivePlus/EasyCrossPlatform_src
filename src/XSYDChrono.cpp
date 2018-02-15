#include <XSYDChrono.h>

double EasyCrossPlatform::Chrono::systemTime()
{
	std::chrono::time_point<std::chrono::system_clock> nowTime = std::chrono::system_clock::now();
	auto nowDuration = nowTime.time_since_epoch();
	double myNowTime = ((double)nowDuration.count()) / 10000000.00;
	return myNowTime;
}

double EasyCrossPlatform::Chrono::accurateTime()
{
	auto nowTime = std::chrono::steady_clock::now();
	auto nowDuration = nowTime.time_since_epoch();
	double myNowTime = ((double)nowDuration.count()) / 1000000000.00;
	return myNowTime;
}
