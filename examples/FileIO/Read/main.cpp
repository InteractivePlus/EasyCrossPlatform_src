#include "EasyCrossPlatform.h"
#include <iostream>
//File Read Test
//Built under Windows. Can be tested under Linux / Unix if you change the file address, and delete the system("pause")
int main(int argc, char** args) {
	//std::string MyFileRst = EasyCrossPlatform::File::FileIO::ReadPartFile("E:\\233.txt", 0, 3);
	//std::string MyFileRst = EasyCrossPlatform::File::FileIO::ReadPartFile("E:\\233.txt", -3, 3);
	std::string MyFileRst = EasyCrossPlatform::File::FileIO::ReadFile("E:\\233.txt");
	std::cout << "读取结果:" << MyFileRst << std::endl;
	system("pause");
	return 0;
}