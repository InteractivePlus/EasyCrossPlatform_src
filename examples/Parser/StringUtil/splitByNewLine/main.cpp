#include <EasyCrossPlatform.h>
#include <iostream>

int main(int argc, char** args) {
	std::vector<std::vector<std::string::size_type,std::string>> myStrings = EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine("Windows New Line\r\nLinux New Line\nMac New Line\r");
	for (auto i = myStrings.begin(); i != myStrings.end(); i++) {
		std::cout << "NewLine:" << (*i).second << std::endl;
	}
	system("pause");
}