#ifndef __XSYDStringUtilFile__
	#define __XSYDStringUtilFile__
	#include <EasyCP_Common.h>
	#include <locale>
	#include <clocale>
	namespace EasyCrossPlatform {
		namespace Parser{
			namespace StringUtil {
				enum newLineType {
					rn,
					r,
					n
				};
				std::vector<std::pair<std::string::size_type,std::string>> splitStringByNewLine(const std::string& StringToDealWith, int Limit = -1);
				std::vector<std::pair<std::string::size_type,std::string>> splitStringByDivisor(const std::string& StringToDealWith, const std::string& Divisor, int Limit = -1);
				std::vector<std::pair<std::wstring::size_type, std::wstring>> splitStringByNewLine(const std::wstring& StringToDealWith, int Limit = -1);
				std::vector<std::pair<std::wstring::size_type, std::wstring>> splitStringByDivisor(const std::wstring& StringToDealWith, const std::wstring& Divisor, int Limit = -1);
				std::string toLower(const std::string& StringToDealWith);
				std::string toUpper(const std::string& StringToDealWith);
				std::string toSentence(const std::string& StringToDealWith); //Let the first letter to be upper-case, but the other to lower.
				std::wstring toLower(const std::wstring& StringToDealWith);
				std::wstring toUpper(const std::wstring& StringToDealWith);
				std::wstring toSentence(const std::wstring& StringToDealWith);
				std::string leftTrim(const std::string& StringToDealWith);
				std::string rightTrim(const std::string& StringToDealWith);
				std::string trim(const std::string& StringToDealWith);
				std::wstring leftTrim(const std::wstring& StringToDealWith);
				std::wstring rightTrim(const std::wstring& StringToDealWith);
				std::wstring trim(const std::wstring& StringToDealWith);
				std::string fromBytes(const std::vector<byte>& Bytes);
				std::vector<byte> toBytes(const std::string& myStr);
				std::wstring wFromBytes(const std::vector<byte>& Bytes);
				std::vector<byte> toBytes(const std::wstring& myStr);
				unsigned int replace(std::string& str, const std::string& find, const std::string& replace, int Limit = -1);
				unsigned int replace(std::wstring& str, const std::wstring& find, const std::wstring& replace, int Limit = -1);
				#if defined(CROSSPLATFORM_OS_IS_WIN)
				const constexpr char* newLineString = "\r\n";
				#elif defined(CROSSPLATFORM_OS_IS_MAC)
				const constexpr char* newLineString = "\r";
				#else //CROSSPLATFORM_OS_IS_UNIX
				const constexpr char* newLineString = "\n";
				#endif
			}
		}
	}
#endif