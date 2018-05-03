#include <EasyCrossPlatform.h>
#include <iostream>


int main(int argc, char** args) {
	//u8这里代表用utf-8编码.
	//双斜杠IGNORE表示遇到错误就跳过, 双斜杠TRANSLIT表示遇到错误就近找一个最像的字符来替换.
	std::string StringToConvert = u8"我操你妈";
	std::cout << "Original Text:" << StringToConvert << std::endl;
	try {
		std::cout << "Converted Text:" << EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToCharset(StringToConvert, "UTF-8", "GBK//IGNORE") << std::endl;
	}
	catch (std::runtime_error e) {
		std::cout << "Cannot Convert" << std::endl;
	}
	catch (std::invalid_argument e) {
		std::cout << "Convert Charset Not Supported" << std::endl;
	}

	system("pause");
	return 0;
}