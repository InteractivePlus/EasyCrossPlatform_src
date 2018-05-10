#include <XSYDFileInfo.h>
#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
double EasyCrossPlatform::File::FileInfo::FileInfoCls::ConvFileTimeToUTCTime(PFILETIME lptime)
{
	FILETIME ft = (*lptime);

	//FileTimeToLocalFileTime(lptime,&ft);
	
	//LONGLONG nLL;

	ULARGE_INTEGER ui;

	ui.LowPart = ft.dwLowDateTime;

	ui.HighPart = ft.dwHighDateTime;

	//nLL = (ft.dwHighDateTime << 32U) + ft.dwLowDateTime;

	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	double actualTime = static_cast<double>(pt);
	return actualTime;
}
#endif

EasyCrossPlatform::File::FileInfo::FileInfos EasyCrossPlatform::File::FileInfo::FileInfoCls::readFileInfo(const std::string & Path)
{
	FileInfos mResult;
	FileTypes mTypes;
	FileInfo::CleanFileTypesVar(mTypes);

	#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	if (!GetFileAttributesEx(Path.c_str(), GetFileExInfoStandard, &wfad)) {
		throw std::runtime_error("Error occured while getting the file property");
	}
	mResult.FileLastAccessedTime = FileInfoCls::ConvFileTimeToUTCTime(&(wfad.ftLastAccessTime));
	mResult.FileLastModifiedTime = FileInfoCls::ConvFileTimeToUTCTime(&(wfad.ftLastWriteTime));
	DWORD dwAttribute = wfad.dwFileAttributes;
	//https://msdn.microsoft.com/en-us/library/windows/desktop/gg258117(v=vs.85).aspx
	
	if (dwAttribute & FILE_ATTRIBUTE_COMPRESSED) {
		mTypes.isCompressed = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY) {
		mTypes.isDirectory = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_ENCRYPTED) {
		mTypes.isEncrypted = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_HIDDEN) {
		mTypes.isHidden = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_NORMAL) {
		mTypes.isNormalFile = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_OFFLINE) {
		mTypes.disabled = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_READONLY) {
		mTypes.isReadOnly = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_REPARSE_POINT) {
		mTypes.isLink = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_SYSTEM) {
		mTypes.isPartOfSystem = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_TEMPORARY) {
		mTypes.isTempFile = true;
	}
	if (dwAttribute & FILE_ATTRIBUTE_ARCHIVE) { //Archieve(Compressed or doc), if not dir, treat as normal file.
		if (!mTypes.isDirectory) {
			mTypes.isNormalFile = true;
		}
	}
	#else //if defined(EASYCROSSPLATFORM_PLATFORM_UNIX
	//https://blog.csdn.net/yasaken/article/details/7407267
	struct stat st;
	int nRev_st = stat(Path.c_str(), &st);
	if (nRev_st < 0) {
		throw std::runtime_error("Error occured while getting the file property");
	}
	mResult.FileLastAccessedTime = static_cast<double>(st.st_atime);
	mResult.FileLastModifiedTime = static_cast<double>(st.st_ctime);
	mode_t sMode = st.st_mode;
	//https://blog.csdn.net/yasi_xi/article/details/9226267
	if (S_ISLNK(sMode)) {
		mTypes.isLink = true;
	}
	if (S_ISREG(sMode)) {
		mTypes.isNormalFile = true;
	}
	if (S_ISDIR(sMode)) {
		mTypes.isDirectory = true;
	}
	if (S_ISSOCK(sMode)) { //IS A Socket File, treat as normal file
		mTypes.isNormalFile = true;
	}
	#endif
	mResult.FileAttrs = mTypes;
	return mResult;
}

std::vector<std::string> EasyCrossPlatform::File::FileInfo::FileInfoCls::getDirectoryFileList(const std::string& DirectoryPath)
{
	std::vector<std::string> files;//存放文件名  
	std::string NSDirPath = DirectoryPath;
	if (NSDirPath[NSDirPath.length() - 1U] != '/' && NSDirPath[NSDirPath.length() - 1U] != '\\') {
		NSDirPath += File::FolderSeparator;
	}

#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS  
	NSDirPath += "*";
	_finddata_t file;
	long lf;
	//输入文件夹路径  
	if ((lf = _findfirst(NSDirPath.c_str(), &file)) == -1) {
		throw std::runtime_error("Directory not found");
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//输出文件名  
			//cout<<file.name<<endl;  
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
#else //EASYCROSSPLATFORM_PLATFORM_UNIX 
	DIR *dir;
	struct dirent *ptr;
	//char base[1000];

	if ((dir = opendir(NSDirPath.c_str())) == NULL)
	{
		throw std::runtime_error("Directory not found");
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir  
			continue;
		else if(ptr->d_type == DT_REG || ptr->d_type == DT_DIR || ptr->d_type == DT_SOCK || ptr->d_type == DT_LNK || ptr->d_type == DT_UNKNOWN)
		{
			files.push_back(ptr->d_name);
			/*
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_nSame);
			readFileList(base);
			*/
		}
	}
	closedir(dir);
#endif  

	//排序，按从小到大排序  
	std::sort(files.begin(), files.end());
	return files;
}

void EasyCrossPlatform::File::FileInfo::CleanFileTypesVar(FileTypes & VarToDeal)
{
	VarToDeal.disabled = false;
	VarToDeal.isCompressed = false;
	VarToDeal.isDirectory = false;
	VarToDeal.isEncrypted = false;
	VarToDeal.isHidden = false;
	VarToDeal.isLink = false;
	VarToDeal.isNormalFile = false;
	VarToDeal.isPartOfSystem = false;
	VarToDeal.isReadOnly = false;
	VarToDeal.isTempFile = false;
	return;
}
