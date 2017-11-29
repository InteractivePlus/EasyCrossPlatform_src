#include "EasyCrossPlatform.h"
#include <iostream>
//File Write Test
//Built under Windows. Can be tested under Linux / Unix if you change the file address, and delete the system("pause")
int main(int argc, char** args) {
	bool WriteRst = EasyCrossPlatform::File::FileIO::WriteFile("E:\\233.txt","23333333");
	if(WriteRst){
		std::cout << "Write Successfully" << std::endl;
	}else{
		std::cout << "Write Failed" << std::endl;
	}
	system("pause");
	return 0;
}