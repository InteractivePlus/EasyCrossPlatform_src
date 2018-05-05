#include <XSYDStringUtil.h>

std::vector<std::pair<std::string::size_type, std::string>> EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine(const std::string & StringToDealWith, int Limit)
{
	if (Limit == 0) {
		return std::vector<std::pair<std::string::size_type, std::string>>();
	}
	std::vector<std::pair<std::string::size_type, std::string>> myResults;
	std::string tmpStr = StringToDealWith;
	std::string::size_type LastFindPlace = std::string::npos;
	bool inited = false;
	newLineType lastNewLineType = r;
	std::string::size_type lastNewLineLength = 0U;
	unsigned int numSplited = 0U;
	std::string::size_type NowAt = 0U;
	while (true) {
		lastNewLineLength = lastNewLineType == rn ? 2U : 1U;
		if (LastFindPlace != std::string::npos) {
			myResults.push_back(std::pair<std::string::size_type,std::string>(NowAt,tmpStr.substr(0U, LastFindPlace)));
			NowAt += LastFindPlace + lastNewLineLength;
			numSplited++;
			if (static_cast<int>(numSplited) >= Limit && Limit != -1) {
				break;
			}
			tmpStr = tmpStr.substr(LastFindPlace + lastNewLineLength, tmpStr.length() - LastFindPlace - lastNewLineLength);
		}
		else {
			if (!inited) {
				inited = true;
			}
			else {
				myResults.push_back(std::pair<std::string::size_type, std::string>(NowAt, tmpStr));
				numSplited++;
				tmpStr = "";
				break;
			}
		}
		std::string::size_type rnPos = tmpStr.find("\r\n", 0U);
		std::string::size_type rPos = tmpStr.find("\r", 0U);
		std::string::size_type nPos = tmpStr.find("\n", 0U);
		if (rPos < rnPos && rPos < nPos) {
			lastNewLineType = r;
			LastFindPlace = rPos;
		}
		else if (nPos < rnPos && nPos < rPos) {
			lastNewLineType = n;
			LastFindPlace = nPos;
		}
		else if(rnPos != std::string::npos){
			lastNewLineType = rn;
			LastFindPlace = rnPos;
		}
		else {
			LastFindPlace = std::string::npos;
			continue;
		}
	}
	return myResults;
}

std::vector<std::pair<std::string::size_type,std::string>> EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(const std::string & StringToDealWith, const std::string & Divisor, int Limit)
{
	std::vector<std::pair<std::string::size_type,std::string>> myTmpResult;
	if (Limit == 0) {
		return myTmpResult;
	}
	else if (Divisor.empty() || StringToDealWith.empty()) {
		return myTmpResult;
	}
	unsigned int numSplited = 0U;
	bool inited = false;
	std::string::size_type LastFindPlace = std::string::npos;
	std::string tmpStr = StringToDealWith;
	std::string::size_type NowAt = 0U;
	while (true) {
		if (LastFindPlace != std::string::npos) {
			myTmpResult.push_back(std::pair<std::string::size_type,std::string>(NowAt,tmpStr.substr(0U, LastFindPlace)));
			numSplited++;
			NowAt += LastFindPlace + Divisor.length();
			if (static_cast<int>(numSplited) >= Limit && Limit != -1) {
				break;
			}
			tmpStr = tmpStr.substr(LastFindPlace + Divisor.length(), tmpStr.length() - LastFindPlace - Divisor.length());
		}
		else {
			if (!inited) {
				inited = true;
			}
			else {
				myTmpResult.push_back(std::pair<std::string::size_type,std::string>(NowAt,tmpStr));
				numSplited++;
				tmpStr = "";
				break;
			}
		}
		LastFindPlace = tmpStr.find(Divisor, 0U);
	}
	return myTmpResult;
}

std::vector<std::pair<std::wstring::size_type, std::wstring>> EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine(const std::wstring & StringToDealWith, int Limit)
{
	if (Limit == 0) {
		return std::vector<std::pair<std::wstring::size_type, std::wstring>>();
	}
	std::vector<std::pair<std::wstring::size_type, std::wstring>> myResults;
	std::wstring tmpStr = StringToDealWith;
	std::wstring::size_type LastFindPlace = std::wstring::npos;
	bool inited = false;
	newLineType lastNewLineType = r;
	std::wstring::size_type lastNewLineLength = 0U;
	unsigned int numSplited = 0U;
	std::wstring::size_type NowAt = 0U;
	while (true) {
		lastNewLineLength = lastNewLineType == rn ? 2U : 1U;
		if (LastFindPlace != std::wstring::npos) {
			myResults.push_back(std::pair<std::wstring::size_type, std::wstring>(NowAt, tmpStr.substr(0U, LastFindPlace)));
			NowAt += LastFindPlace + lastNewLineLength;
			numSplited++;
			if (static_cast<int>(numSplited) >= Limit && Limit != -1) {
				break;
			}
			tmpStr = tmpStr.substr(LastFindPlace + lastNewLineLength, tmpStr.length() - LastFindPlace - lastNewLineLength);
		}
		else {
			if (!inited) {
				inited = true;
			}
			else {
				myResults.push_back(std::pair<std::wstring::size_type, std::wstring>(NowAt, tmpStr));
				numSplited++;
				tmpStr = L"";
				break;
			}
		}
		std::wstring::size_type rnPos = tmpStr.find(L"\r\n", 0U);
		std::wstring::size_type rPos = tmpStr.find(L"\r", 0U);
		std::wstring::size_type nPos = tmpStr.find(L"\n", 0U);
		if (rPos < rnPos && rPos < nPos) {
			lastNewLineType = r;
			LastFindPlace = rPos;
		}
		else if (nPos < rnPos && nPos < rPos) {
			lastNewLineType = n;
			LastFindPlace = nPos;
		}
		else if (rnPos != std::wstring::npos) {
			lastNewLineType = rn;
			LastFindPlace = rnPos;
		}
		else {
			LastFindPlace = std::wstring::npos;
			continue;
		}
	}
	return myResults;
}

std::vector<std::pair<std::wstring::size_type, std::wstring>> EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(const std::wstring & StringToDealWith, const std::wstring & Divisor, int Limit)
{
	std::vector<std::pair<std::wstring::size_type, std::wstring>> myTmpResult;
	if (Limit == 0) {
		return myTmpResult;
	}
	else if (Divisor.empty() || StringToDealWith.empty()) {
		return myTmpResult;
	}
	unsigned int numSplited = 0U;
	bool inited = false;
	std::wstring::size_type LastFindPlace = std::wstring::npos;
	std::wstring tmpStr = StringToDealWith;
	std::wstring::size_type NowAt = 0U;
	while (true) {
		if (LastFindPlace != std::wstring::npos) {
			myTmpResult.push_back(std::pair<std::wstring::size_type, std::wstring>(NowAt, tmpStr.substr(0U, LastFindPlace)));
			numSplited++;
			NowAt += LastFindPlace + Divisor.length();
			if (static_cast<int>(numSplited) >= Limit && Limit != -1) {
				break;
			}
			tmpStr = tmpStr.substr(LastFindPlace + Divisor.length(), tmpStr.length() - LastFindPlace - Divisor.length());
		}
		else {
			if (!inited) {
				inited = true;
			}
			else {
				myTmpResult.push_back(std::pair<std::wstring::size_type, std::wstring>(NowAt, tmpStr));
				numSplited++;
				tmpStr = L"";
				break;
			}
		}
		LastFindPlace = tmpStr.find(Divisor, 0U);
	}
	return myTmpResult;
}

std::string EasyCrossPlatform::Parser::StringUtil::toLower(const std::string & StringToDealWith)
{
	std::string myNewStr = StringToDealWith;
	std::transform(StringToDealWith.begin(), StringToDealWith.end(), myNewStr.begin(), ::tolower);
	return myNewStr;
}

std::string EasyCrossPlatform::Parser::StringUtil::toUpper(const std::string & StringToDealWith)
{
	std::string myNewStr = StringToDealWith;
	std::transform(StringToDealWith.begin(), StringToDealWith.end(), myNewStr.begin(), ::toupper);
	return myNewStr;
}

std::string EasyCrossPlatform::Parser::StringUtil::toSentence(const std::string & StringToDealWith)
{
	std::string myNewStr = StringToDealWith;
	myNewStr = toLower(myNewStr);
	if (myNewStr.size() >= 1U) {
		if (myNewStr.size() >= 2U) {
			myNewStr = toUpper(myNewStr.substr(0U, 1U)) + myNewStr.substr(1U, myNewStr.size() - 1U);
		}
		else {
			myNewStr = toUpper(myNewStr);
		}
	}
	return myNewStr;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::toLower(const std::wstring & StringToDealWith)
{
	std::wstring myNewStr = StringToDealWith;
	std::transform(StringToDealWith.begin(), StringToDealWith.end(), myNewStr.begin(), ::tolower);
	return myNewStr;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::toUpper(const std::wstring & StringToDealWith)
{
	std::wstring myNewStr = StringToDealWith;
	std::transform(StringToDealWith.begin(), StringToDealWith.end(), myNewStr.begin(), ::toupper);
	return myNewStr;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::toSentence(const std::wstring & StringToDealWith)
{
	std::wstring myNewStr = StringToDealWith;
	myNewStr = toLower(myNewStr);
	if (myNewStr.size() >= 1U) {
		if (myNewStr.size() >= 2U) {
			myNewStr = toUpper(myNewStr.substr(0U, 1U)) + myNewStr.substr(1U, myNewStr.size() - 1U);
		}
		else {
			myNewStr = toUpper(myNewStr);
		}
	}
	return myNewStr;
}

std::string EasyCrossPlatform::Parser::StringUtil::leftTrim(const std::string & StringToDealWith)
{
	std::string mString = StringToDealWith;
	std::string::size_type FirstNotSpace = mString.find_first_not_of(' ', 0U);
	if (FirstNotSpace != std::string::npos && FirstNotSpace > 0U) {
		mString = mString.substr(FirstNotSpace, mString.length() - FirstNotSpace);
	}
	return mString;
}

std::string EasyCrossPlatform::Parser::StringUtil::rightTrim(const std::string & StringToDealWith)
{
	std::string mString = StringToDealWith;
	std::string::size_type LastNotSpace = mString.find_last_not_of(' ', mString.length());
	if (LastNotSpace != std::string::npos && LastNotSpace < mString.length() - 1U) {
		mString = mString.substr(0U, LastNotSpace+1);
	}
	return mString;
}

std::string EasyCrossPlatform::Parser::StringUtil::trim(const std::string & StringToDealWith)
{
	return leftTrim(rightTrim(StringToDealWith));
}

std::wstring EasyCrossPlatform::Parser::StringUtil::leftTrim(const std::wstring & StringToDealWith)
{
	std::wstring mString = StringToDealWith;
	std::wstring::size_type FirstNotSpace = mString.find_first_not_of(L' ', 0U);
	if (FirstNotSpace != std::wstring::npos && FirstNotSpace > 0U) {
		mString = mString.substr(FirstNotSpace, mString.length() - FirstNotSpace);
	}
	return mString;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::rightTrim(const std::wstring & StringToDealWith)
{
	std::wstring mString = StringToDealWith;
	std::wstring::size_type LastNotSpace = mString.find_last_not_of(L' ', mString.length());
	if (LastNotSpace != std::wstring::npos && LastNotSpace < mString.length() - 1U) {
		mString = mString.substr(0U, LastNotSpace + 1U);
	}
	return mString;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::trim(const std::wstring & StringToDealWith)
{
	return leftTrim(rightTrim(StringToDealWith));
}

std::string EasyCrossPlatform::Parser::StringUtil::fromBytes(const std::vector<byte>& Bytes)
{
	std::string mResult = std::string();
	const unsigned int SizeOfChar = sizeof(char);
	byte mWcharArray[SizeOfChar];
	unsigned int TotalWCharNum = Bytes.size() / SizeOfChar;
	mResult.reserve(TotalWCharNum);
	for (unsigned int i = 0U; i < TotalWCharNum; i++) {
		for (unsigned int j = 0U; j < SizeOfChar; j++) {
			mWcharArray[j] = Bytes[i*SizeOfChar + j];
		}
		mResult += StringUtil::ConvertFrom<char>(mWcharArray);
	}
	return mResult;
}

std::vector<byte> EasyCrossPlatform::Parser::StringUtil::toBytes(const std::string & myStr)
{
	std::vector<byte> mBytes = std::vector<byte>();
	const unsigned int SizeOfChar = sizeof(char);
	byte mByteArray[SizeOfChar];
	unsigned int TotalByteNum = myStr.length() * SizeOfChar;
	unsigned int TotalCharNum = myStr.length();
	mBytes.reserve(TotalByteNum);
	for (unsigned int i = 0U; i < TotalCharNum; i++) {
		StringUtil::ConvertTo(myStr[i], mByteArray);
		for (byte tempByte : mByteArray) {
			mBytes.push_back(tempByte);
		}
	}
	return mBytes;
}

std::u16string EasyCrossPlatform::Parser::StringUtil::u16FromBytes(const std::vector<byte>& Bytes)
{
	std::u16string mResult = std::u16string();
	const unsigned int SizeOfChar = sizeof(char16_t);
	byte mWcharArray[SizeOfChar];
	unsigned int TotalWCharNum = Bytes.size() / SizeOfChar;
	mResult.reserve(TotalWCharNum);
	for (unsigned int i = 0U; i < TotalWCharNum; i++) {
		for (unsigned int j = 0U; j < SizeOfChar; j++) {
			mWcharArray[j] = Bytes[i*SizeOfChar + j];
		}
		mResult += StringUtil::ConvertFrom<char16_t>(mWcharArray);
	}
	return mResult;
}

std::vector<byte> EasyCrossPlatform::Parser::StringUtil::toBytes(const std::u16string & myStr)
{
	std::vector<byte> mBytes = std::vector<byte>();
	const unsigned int SizeOfChar = sizeof(char16_t);
	byte mByteArray[SizeOfChar];
	unsigned int TotalByteNum = myStr.length() * SizeOfChar;
	unsigned int TotalCharNum = myStr.length();
	mBytes.reserve(TotalByteNum);
	for (unsigned int i = 0U; i < TotalCharNum; i++) {
		StringUtil::ConvertTo(myStr[i], mByteArray);
		for (byte tempByte : mByteArray) {
			mBytes.push_back(tempByte);
		}
	}
	return mBytes;
}

std::u32string EasyCrossPlatform::Parser::StringUtil::u32FromBytes(const std::vector<byte>& Bytes)
{
	std::u32string mResult = std::u32string();
	const unsigned int SizeOfChar = sizeof(char32_t);
	byte mWcharArray[SizeOfChar];
	unsigned int TotalWCharNum = Bytes.size() / SizeOfChar;
	mResult.reserve(TotalWCharNum);
	for (unsigned int i = 0U; i < TotalWCharNum; i++) {
		for (unsigned int j = 0U; j < SizeOfChar; j++) {
			mWcharArray[j] = Bytes[i*SizeOfChar + j];
		}
		mResult += StringUtil::ConvertFrom<char32_t>(mWcharArray);
	}
	return mResult;
}

std::vector<byte> EasyCrossPlatform::Parser::StringUtil::toBytes(const std::u32string & myStr)
{
	std::vector<byte> mBytes = std::vector<byte>();
	const unsigned int SizeOfChar = sizeof(char32_t);
	byte mByteArray[SizeOfChar];
	unsigned int TotalByteNum = myStr.length() * SizeOfChar;
	unsigned int TotalCharNum = myStr.length();
	mBytes.reserve(TotalByteNum);
	for (unsigned int i = 0U; i < TotalCharNum; i++) {
		StringUtil::ConvertTo(myStr[i], mByteArray);
		for (byte tempByte : mByteArray) {
			mBytes.push_back(tempByte);
		}
	}
	return mBytes;
}

std::wstring EasyCrossPlatform::Parser::StringUtil::wFromBytes(const std::vector<byte>& Bytes)
{
	std::wstring mResult = std::wstring();
	const unsigned int SizeOfWChar = sizeof(wchar_t);
	byte mWcharArray[SizeOfWChar];
	unsigned int TotalWCharNum = Bytes.size() / SizeOfWChar;
	mResult.reserve(TotalWCharNum);
	for (unsigned int i = 0U; i < TotalWCharNum; i++) {
		for (unsigned int j = 0U; j < SizeOfWChar; j++) {
			mWcharArray[j] = Bytes[i*SizeOfWChar + j];
		}
		mResult += StringUtil::ConvertFrom<wchar_t>(mWcharArray);
	}
	return mResult;
}

std::vector<byte> EasyCrossPlatform::Parser::StringUtil::toBytes(const std::wstring & myStr)
{
	std::vector<byte> mBytes = std::vector<byte>();
	const unsigned int SizeOfWChar = sizeof(wchar_t);
	byte mByteArray[SizeOfWChar];
	unsigned int TotalByteNum = myStr.length() * SizeOfWChar;
	unsigned int TotalWCharNum = myStr.length();
	mBytes.reserve(TotalByteNum);
	for (unsigned int i = 0U; i < TotalWCharNum; i++) {
		StringUtil::ConvertTo(myStr[i], mByteArray);
		for (byte tempByte : mByteArray) {
			mBytes.push_back(tempByte);
		}
	}
	return mBytes;
}

unsigned int EasyCrossPlatform::Parser::StringUtil::replace(std::string & str, const std::string & find, const std::string & replace, int Limit, std::string::size_type Start)
{
	unsigned int NumReplaced = 0U;
	std::string::size_type lastPos = 0U;
	std::string::size_type startPos = Start;
	while (Limit == -1 || NumReplaced < static_cast<unsigned int>(Limit)) {
		startPos = str.find(find,lastPos);
		if (startPos == std::string::npos) {
			break;
		}
		str.replace(startPos, find.length(), replace);
		lastPos = startPos + replace.length();
		NumReplaced++;
	}
	return NumReplaced;
}

unsigned int EasyCrossPlatform::Parser::StringUtil::replace(std::wstring & str, const std::wstring & find, const std::wstring & replace, int Limit, std::wstring::size_type Start)
{
	unsigned int NumReplaced = 0U;
	std::wstring::size_type lastPos = 0U;
	std::wstring::size_type startPos = Start;
	while (Limit == -1 || NumReplaced < static_cast<unsigned int>(Limit)) {
		startPos = str.find(find, lastPos);
		if (startPos == std::wstring::npos) {
			break;
		}
		str.replace(startPos, find.length(), replace);
		lastPos = startPos + replace.length();
		NumReplaced++;
	}
	return NumReplaced;
}