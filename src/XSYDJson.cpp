#include <XSYDJson.h>

Json::Value EasyCrossPlatform::Parser::JSON::ReadFromJSON(const std::string& JSONString)
{
	Json::Value MyReadResult;
	Json::Reader MyReader;
	if (!MyReader.parse(JSONString, MyReadResult, false)) {
		//Error occured
		throw std::runtime_error("This JSON string cannot be parsed");
	}
	return MyReadResult;
}

std::string EasyCrossPlatform::Parser::JSON::WriteToJSON(const Json::Value& JSONObj) {
	Json::FastWriter MyWriter;
	std::string MyResult = MyWriter.write(JSONObj);
	return MyResult;
}
