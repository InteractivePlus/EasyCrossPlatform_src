#ifndef __EasyCrossPlatform_RegexFile__
	#define __EasyCrossPlatform_RegexFile__
	#include <EasyCP_Common.h>
	#include <regex>
	namespace EasyCrossPlatform {
		namespace Parser {
			namespace Regex {
				struct RegexInfo {
					std::string MatchedText;
					std::string::size_type StartIndex;
				};
				std::vector<RegexInfo> MatchRegex(const std::string& TextToMatch, const std::string& RegexExpr);
				std::vector<RegexInfo> SingleSearchRegex(const std::string& TextToMatch, const std::string& RegexExpr);
				std::vector<std::vector<RegexInfo>> MultipleSearchRegex(const std::string& TextToMatch, const std::string& RegexExpr, const int Limit = -1);
				std::string ReplaceRegexWithString(const std::string& Text, const std::string& RegexExpr, const std::string& ReplaceStr, const int Limit = -1);
			}
		}
	}
#endif