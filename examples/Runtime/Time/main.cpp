#include "EasyCrossPlatform.h"
#include <iostream>
//ChronoTest
//Built Under Windows, can be tested under Unix/Linux if system("pause") is removed.
int main(int argc, char** args) {
	//double nowTime = EasyCrossPlatform::Chrono::systemTime();
	double nowTime = EasyCrossPlatform::Chrono::accurateTime();
	std::cout << "Time:" << nowTime << std::endl;
	system("pause");
	//double newTime = EasyCrossPlatform::Chrono::systemTime();
	double newTime = EasyCrossPlatform::Chrono::accurateTime();
	std::cout << "Duration: " << (newTime - nowTime) << " s" << std::endl;
	std::cout << "Take: " << (2 * sizeof(double)) << "Byte Space" << std::endl;
	system("pause");
}