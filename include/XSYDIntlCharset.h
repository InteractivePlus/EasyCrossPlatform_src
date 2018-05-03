#ifndef __XSYDInternationalCharsetFile__
	#define __XSYDInternationalCharsetFile__
	#include <EasyCP_Common.h>
	#include <iconv/iconv.h>
	#include <XSYDStringUtil.h>
	namespace EasyCrossPlatform {
		namespace Parser {
			namespace StringUtil {
				namespace Charset {
					std::string ConvertToCharset(const std::string& InText, const std::string& InTextCharset, const std::string& OutTextCharset); //Throws Runtime Error when encoutering error.
					std::u16string ConvertToUTF16(const std::string& InText, const std::string& InTextCharset);
					std::u32string ConvertToUTF32(const std::string& InText, const std::string& InTextCharset);
					std::wstring ConvertToWString(const std::string& InText, const std::string& InTextCharset);
				}
			}
		}
	}
#endif