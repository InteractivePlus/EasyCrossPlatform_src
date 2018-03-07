#ifndef __XSYDFileIOFile__
	#define __XSYDFileIOFile__
	#include <EasyCP_Common.h>
	#include <stdio.h>
	namespace EasyCrossPlatform {
		namespace File {
			class FileIO {
			private:
				static FILE* OpenFile(const char* Path, const char * Mode);
				static bool BinaryRead(void* _Buffer, unsigned int ElementSize, unsigned int _BufferNum, FILE* MyFile);
				static bool BinaryWrite(void* _Buffer, unsigned int ElementSize, unsigned int _BufferNum, FILE* MyFile);
			public:
				static std::string ReadPartFile(const std::string &Path, const long StartPos, const unsigned long ReadLength);
				static std::string ReadPartFile(const char* Path, const long StartPos, const unsigned long ReadLength);
				static bool BinaryReadFile(const std::string &Path, const long StartPos, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static bool BinaryReadFile(const char* Path, const long StartPos, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static std::string ReadFile(const std::string &Path);
				static std::string ReadFile(const char* Path);
				static bool WriteFile_ReplaceAt(long BytesMoved, const std::string &Path, const std::string &Content);
				static bool WriteFile_ReplaceAt(long BytesMoved, const char * Path, const std::string &Content);
				static bool WriteFile(const std::string &Path, const std::string &Content);
				static bool WriteFile(const char* Path, const std::string &Content);
				static bool BinaryWriteFile(const std::string &Path, const long StartPos, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static bool BinaryWriteFile(const char* Path, const long StartPos, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static bool WriteFile_Append(const std::string &Path, const std::string &Content);
				static bool WriteFile_Append(const char* Path, const std::string &Content);
				static bool BinaryWriteFile_Append(const std::string &Path, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static bool BinaryWriteFile_Append(const char* Path, void* _Buffer, unsigned int ElementSize, unsigned int Count);
				static unsigned long GetFileLength(const std::string &Path);
				static unsigned long GetFileLength(const char* Path);
			};
		}
	}
#endif