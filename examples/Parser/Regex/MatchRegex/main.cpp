#include <EasyCrossPlatform.h>
#include <iostream>


int main(int argc, char** args) {
	std::string mText = "subject";
	std::string mRegex = "(sub)(.*)";
	std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo> mMatchedResult = EasyCrossPlatform::Parser::Regex::MatchRegex(mText, mRegex);
	for (EasyCrossPlatform::Parser::Regex::RegexInfo mInfo : mMatchedResult) {
		std::cout << "Text:" + mInfo.MatchedText << std::endl;
		std::cout << "StartPos:" << mInfo.StartIndex << std::endl;
		std::cout << "--------------------" << std::endl;
	}
	system("pause");
	return 0;
}