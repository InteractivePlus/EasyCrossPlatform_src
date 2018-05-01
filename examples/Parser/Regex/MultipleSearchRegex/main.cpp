#include <EasyCrossPlatform.h>
#include <iostream>


int main(int argc, char** args) {
	std::string mText = "Date:2017-10-10 ~ 2017-10-15";
	std::string mRegex = "(\\d{4})-(\\d{2}-(\\d{2}))";
	std::vector<std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo>> mMatchedResults = EasyCrossPlatform::Parser::Regex::MultipleSearchRegex(mText, mRegex);
	for (std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo> mInfos : mMatchedResults) {
		for (EasyCrossPlatform::Parser::Regex::RegexInfo mInfo : mInfos) {
			std::cout << "Text:" + mInfo.MatchedText << std::endl;
			std::cout << "StartPos:" << mInfo.StartIndex << std::endl;
			std::cout << std::endl;
		}
		std::cout << "---------------------------" << std::endl;
	}
	system("pause");
	return 0;
}