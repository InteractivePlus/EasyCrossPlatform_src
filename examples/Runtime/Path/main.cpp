#include <EasyCrossPlatform.h>
#include <iostream>
int main(int argc, char** args) {
	std::string myPath = EasyCrossPlatform::Runtime::Path::APPPath();
	std::string myExeName = EasyCrossPlatform::Runtime::Path::APPExeName();
	std::cout << "Path: " << myPath << std::endl;
	std::cout << "Exe Name: " << myExeName << std::endl;
	system("pause");
}