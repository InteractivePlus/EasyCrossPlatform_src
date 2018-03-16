#include <XSYDAPPRuntime.h>

std::string EasyCrossPlatform::Runtime::Path::APPPath()
{
	int ExePathLength = wai_getExecutablePath(NULL, 0, NULL);
	char* myPath = new char[ExePathLength+1];
	int DirPathLength = 0;
	wai_getExecutablePath(myPath, ExePathLength, &DirPathLength);
	myPath[ExePathLength] = '\0';
	std::string mPath = std::string(myPath, DirPathLength+1);
	delete[] myPath;
	return mPath;
}

std::string EasyCrossPlatform::Runtime::Path::APPExeName()
{
	int ExePathLength = wai_getExecutablePath(NULL, 0, NULL);
	char* myPath = new char[ExePathLength + 1];
	int DirPathLength = 0;
	wai_getExecutablePath(myPath, ExePathLength, &DirPathLength);
	myPath[ExePathLength] = '\0';
	std::string mPath = std::string(myPath + DirPathLength + 1, ExePathLength - DirPathLength - 1);
	delete[] myPath;
	return mPath;
}
