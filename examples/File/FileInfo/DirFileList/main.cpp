#include <EasyCrossPlatform.h>
#include <iostream>
int main(int argc, char** args) {
	std::string APPPath = EasyCrossPlatform::Runtime::Path::APPPath();
	std::string CWDPath = EasyCrossPlatform::Runtime::Path::CWDPath();
	std::vector<std::string> PathFileList = EasyCrossPlatform::File::FileInfo::FileInfoCls::getDirectoryFileList(CWDPath);
	std::cout << "Path: " << CWDPath << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	for (std::string i : PathFileList) {
		std::cout << i << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
	system("pause");
}