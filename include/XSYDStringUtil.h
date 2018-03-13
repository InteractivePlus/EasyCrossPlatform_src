#ifndef __XSYDStringUtilFile__
	#define __XSYDStringUtilFile__
	#include <EasyCP_Common.h>
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
				std::string toLower(const std::string& StringToDealWith);
				std::string toUpper(const std::string& StringToDealWith);
				std::string fromBytes(const std::vector<byte>& Bytes);
				std::vector<byte> toBytes(const std::string& myStr);
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