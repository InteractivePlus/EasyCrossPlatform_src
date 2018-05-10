#include <XSYDAPPRuntime.h>

std::string EasyCrossPlatform::Runtime::Path::APPPath()
{
	int ExePathLength = wai_getExecutablePath(NULL, 0, NULL);
	char* myPath = new char[ExePathLength + 1];
	int DirPathLength = 0;
	wai_getExecutablePath(myPath, ExePathLength, &DirPathLength);
	myPath[ExePathLength] = '\0';
	std::string mPath = std::string(myPath, DirPathLength);
	if (mPath[mPath.length() - 1U] == '\\' || mPath[mPath.length() - 1U] == '/') {
		//Directory Separator Ended.
	}
	else {
		#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
			mPath += "\\";
		#else //if EASYCROSSPLATFORM_PLATFORM_UNIX
			mPath += "/";
		#endif
	}
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

std::string EasyCrossPlatform::Runtime::Path::CWDPath()
{
	char buffer[Path::PathMaxSize];
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	if (_getcwd(buffer, Path::PathMaxSize) == NULL) {
#else //if EASYCROSSPLATFORM_PLATFORM_UNIX
	if (getcwd(buffer, Path::PathMaxSize) == NULL) {
#endif
		throw std::runtime_error("Encountered an error while getting the current working directory.");
	}
	std::string mResult = std::string(buffer);
	if (mResult[mResult.length() - 1U] == '\\' || mResult[mResult.length() - 1U] == '/') {
		//Directory Separator Ended
	}
	else {
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
		mResult += "\\";
#else //if EASYCROSSPLATFORM_PLATFORM_UNIX
		mResult += "/";
#endif
	}
	return mResult;
}