#include <EasyCrossPlatform.h>
#include <iostream>


int main(int argc, char** args) {
	std::string ConvBefore = u8"你好, Hello, Hallo, ハロー,привет,Bonjour,Hallå";
	std::wstring ConvAfter = EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToWString(ConvBefore, "UTF-8");
	std::wcout << ConvAfter << std::endl;
	system("pause");
	return 0;
}