#ifndef __EasyCrossPlatform_FileInfoFile__
	#define __EasyCrossPlatform_FileInfoFile__
	#include <EasyCP_Common.h>
	#include <chrono>
	#include <ctime>
	//#include <filewatcher/FileWatch.hpp> //Add FileWatcher Functionality
	#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
		#include <Windows.h>
	#else //if defined(EASYCROSSPLATFORM_PLATFORM_UNIX)
		#include <sys/stat.h>  
		#include <fcntl.h>  
		#include <stdio.h> 
		#include <time.h> 
		#include <unistd.h>
	#endif
	namespace EasyCrossPlatform {
		namespace File {
			#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
			const constexpr char* FolderSeparator = "\\";
			#else //if defined(EASYCROSSPLATFORM_PLATFORM_UNIX)
			const constexpr char* FolderSeparator = "/";
			#endif
			namespace FileInfo {
				struct FileTypes {
					bool isDirectory;
					bool isNormalFile;
					bool isCompressed;
					bool isEncrypted;
					bool isHidden;
					bool disabled; //OFFLINE,
					bool isReadOnly;
					bool isLink;
					bool isPartOfSystem;
					bool isTempFile;
				};
				struct FileInfos {
					FileTypes FileAttrs;
					double FileLastAccessedTime;
					double FileLastModifiedTime;
				};
				void CleanFileTypesVar(FileTypes& VarToDeal);

				class FileInfoCls {
				protected:
					#ifdef EASYCROSSPLATFORM_PLATFORM_WINDOWS
					static double ConvFileTimeToUTCTime(PFILETIME lptime);
					#endif
				public:
					static FileInfos readFileInfo(const std::string& Path);
				};
			}
		}
	}
#endif