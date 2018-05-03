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
	const char* encodeFrom = InTextCharset.c_str();
	iconv_t mIconv = iconv_open(encodeTo, encodeFrom);
	if (mIconv == (iconv_t)-1) {
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
	size_t mConvResult = iconv(mIconv, (const char**)&inBuf, &TempInBufLength, &outBuf, &TempOutBufLength);
	if (mConvResult == (size_t)-1) {
		throw std::runtime_error("Some character conversion failed");
	}
	char16_t* OutBufWCharPtr = reinterpret_cast<char16_t*>(outBufPtr);
	std::u16string mResult = std::u16string(OutBufWCharPtr, (OutBufLength - TempOutBufLength)/sizeof(char16_t));
	delete[] inBufPtr;
	delete[] outBufPtr;
	iconv_close(mIconv);
	return mResult;
}


std::u32string EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToUTF32(const std::string & InText, const std::string & InTextCharset)
{
	const char* encodeTo = "UTF-32//IGNORE";
	const char* encodeFrom = InTextCharset.c_str();
	iconv_t mIconv = iconv_open(encodeTo, encodeFrom);
	if (mIconv == (iconv_t)-1) {
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
	size_t mConvResult = iconv(mIconv, (const char**)&inBuf, &TempInBufLength, &outBuf, &TempOutBufLength);
	if (mConvResult == (size_t)-1) {
		throw std::runtime_error("Some character conversion failed");
	}
	char32_t* OutBufWCharPtr = reinterpret_cast<char32_t*>(outBufPtr);
	std::u32string mResult = std::u32string(OutBufWCharPtr, (OutBufLength - TempOutBufLength) / sizeof(char32_t));
	delete[] inBufPtr;
	delete[] outBufPtr;
	iconv_close(mIconv);
	return mResult;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::Charset::ConvertToWString(const std::string & InText, const std::string & InTextCharset)
{
	std::u16string Converted16String;
	std::u32string Converted32String;
	std::wstring mResult;
	wchar_t* newWCharArr;
	switch (sizeof(wchar_t)) {
	case 2U:
		try {
			 Converted16String = ConvertToUTF16(InText, InTextCharset);
		}
		catch (std::runtime_error e) {
			throw e;
		}
		catch (std::invalid_argument e) {
			throw e;
		}
		newWCharArr = new wchar_t[Converted16String.length()];
		for (unsigned int i = 0U; i < Converted16String.length(); i++) {
			newWCharArr[i] = static_cast<wchar_t>(Converted16String[i]);
		}
		mResult.assign(newWCharArr, Converted16String.length());
		break;
	case 4U:
		try {
			Converted32String = ConvertToUTF32(InText, InTextCharset);
		}
		catch (std::runtime_error e) {
			throw e;
		}
		catch (std::invalid_argument e) {
			throw e;
		}
		newWCharArr = new wchar_t[Converted32String.length()];
		for (unsigned int i = 0U; i < Converted32String.length(); i++) {
			newWCharArr[i] = static_cast<wchar_t>(Converted32String[i]);
		}
		mResult.assign(newWCharArr, Converted32String.length());
		break;
	default:
		throw std::runtime_error("Unknown Wide Char Length");
		break;
	}
	delete[] newWCharArr;
	return mResult;
}
