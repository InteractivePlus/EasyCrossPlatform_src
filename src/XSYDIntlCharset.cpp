#include <XSYDIntlCharset.h>

std::string EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToCharset(const std::string & InText, const std::string & InTextCharset, const std::string & OutTextCharset)
{
	const char* encodeTo = OutTextCharset.c_str();
	const char* encodeFrom = InTextCharset.c_str();
	iconv_t mIconv = iconv_open(encodeTo, encodeFrom);
	if (mIconv == (iconv_t) -1) {
		throw std::invalid_argument("Cannot Create Iconv Handle");
	}
	unsigned int InBufLength = InText.length();
	unsigned int OutBufLength = InBufLength * 4U;
	char* inBuf = new char[InBufLength + 1U];
	char* outBuf = new char[OutBufLength + 1U];
	//memset(inBuf, 0, InBufLength+1U);
	//memset(outBuf, 0, OutBufLength + 1U);
	for (unsigned int i = 0U; i < InBufLength; i++) {
		inBuf[i] = InText[i];
	}
	size_t TempInBufLength = InBufLength;
	size_t TempOutBufLength = OutBufLength;
	char* inBufPtr = inBuf;
	char* outBufPtr = outBuf;
	size_t mConvResult = iconv(mIconv, (const char**) &inBuf, &TempInBufLength, &outBuf, &TempOutBufLength);
	if (mConvResult == (size_t) -1) {
		throw std::runtime_error("Some character conversion failed");
	}
	std::string mResult = std::string(outBufPtr, OutBufLength - TempOutBufLength);
	delete[] inBufPtr;
	delete[] outBufPtr;
	iconv_close(mIconv);
	return mResult;
}

std::u16string EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToUTF16(const std::string & InText, const std::string & InTextCharset)
{	
	const char* encodeTo = "UTF-16//IGNORE";
	std::string ConvRst;
	try {
		ConvRst = ConvertToCharset(InText, InTextCharset, encodeTo);
	}
	catch (std::runtime_error e) {
		throw e;
	}
	catch (std::invalid_argument e) {
		throw e;
	}
	std::vector<byte> tempByte = EasyCrossPlatform::Parser::StringUtil::toBytes(ConvRst);
	std::u16string tempRst = EasyCrossPlatform::Parser::StringUtil::u16FromBytes(tempByte);
	return tempRst;
}


std::u32string EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToUTF32(const std::string & InText, const std::string & InTextCharset)
{
	const char* encodeTo = "UTF-32//IGNORE";
	std::string ConvRst;
	try {
		ConvRst = ConvertToCharset(InText, InTextCharset, encodeTo);
	}
	catch (std::runtime_error e) {
		throw e;
	}
	catch (std::invalid_argument e) {
		throw e;
	}
	std::vector<byte> tempByte = EasyCrossPlatform::Parser::StringUtil::toBytes(ConvRst);
	std::u32string tempRst = EasyCrossPlatform::Parser::StringUtil::u32FromBytes(tempByte);
	return tempRst;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToWString(const std::string & InText, const std::string & InTextCharset)
{
	unsigned int SizeOfWChar = sizeof(wchar_t);
	char* encodeTo = "";
	switch (SizeOfWChar) {
	case 2U:
		encodeTo = "UTF-16//IGNORE";
		break;
	case 3U:
		encodeTo = "UTF-32//IGNORE";
		break;
	default:
		throw std::runtime_error("Your machine's wchar_t size is invalid");
		break;
	}
	std::string ConvRst;
	try {
		ConvRst = ConvertToCharset(InText, InTextCharset, encodeTo);
	}
	catch (std::runtime_error e) {
		throw e;
	}
	catch (std::invalid_argument e) {
		throw e;
	}
	std::vector<byte> tempByte = EasyCrossPlatform::Parser::StringUtil::toBytes(ConvRst);
	std::wstring tempRst = EasyCrossPlatform::Parser::StringUtil::wFromBytes(tempByte);
	return tempRst;
}
