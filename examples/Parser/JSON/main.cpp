#include "EasyCrossPlatform.h"
#include <iostream>
//JSON Test
//Can Be Built Under Windows, if you remove the system(), then it works on unix / linux

int main(int argc, char** args) {
	std::string MyJSON = "{\"XSYD\":\"666\",\"JIBA\":\"DA\"}";
	Json::Value MyValue = EasyCrossPlatform::Parser::JSON::ReadFromJSON(MyJSON);
	std::vector<std::string> MemberNames = MyValue.getMemberNames();
	for(auto i = MemberNames.begin(); i != MemberNames.end(); i++){
		std::cout << (*i) << ":" << MyValue[(*i)].asString() << std::endl;
	}
	system("pause");
	return 0;
}