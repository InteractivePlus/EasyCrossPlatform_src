#include <EasyCrossPlatform.h>
#include <iostream>

int main(int argc, char** args) {
	std::vector<std::pair<std::string::size_type,std::string>> myStrings = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor("233|455", "|");
	for (auto i = myStrings.begin(); i != myStrings.end(); i++) {
		std::cout << "Split:" << (*i).second << std::endl;
	}
	system("pause");
}