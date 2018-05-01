#include <EasyCrossPlatform.h>
#include <iostream>


int main(int argc, char** args) {
	std::string mText = "Date:2017-10-10 ~ 2017-10-15";
	std::string mRegex = "(\\d{4})-(\\d{2}-(\\d{2}))";
	std::string ReplacedString = EasyCrossPlatform::Parser::Regex::ReplaceRegexWithString(mText, mRegex, "XSYDDATE66");
	std::cout << ReplacedString << std::endl;
	system("pause");
	return 0;
}