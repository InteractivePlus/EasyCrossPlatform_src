#include <EasyCrossPlatform.h>
#include <iostream>
int main(int argc, char** args) {
	std::string Path = EasyCrossPlatform::Runtime::Path::APPPath() + "233.txt";
	EasyCrossPlatform::File::FileInfo::FileInfos mInfo = EasyCrossPlatform::File::FileInfo::FileInfos();
	try {
		mInfo = EasyCrossPlatform::File::FileInfo::FileInfoCls::readFileInfo(Path);
	}
	catch (std::runtime_error e) {
		std::cout << "File does not exist." << std::endl;
	}
	std::cout << "File: " << Path << std::endl;
	std::cout << "Last Access Time: " << mInfo.FileLastAccessedTime << std::endl;
	std::cout << "Last Modified Time: " << mInfo.FileLastModifiedTime << std::endl;
	std::cout << "isNormal: " << (mInfo.FileAttrs.isNormalFile ? "Y" : "N") << std::endl;
	std::cout << "isDir: " << (mInfo.FileAttrs.isDirectory ? "Y" : "N") << std::endl;
	system("pause");
}