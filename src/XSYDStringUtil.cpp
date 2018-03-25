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

std::string EasyCrossPlatform::Parser::StringUtil::leftTrim(const std::string & StringToDealWith)
{
	std::string mString = StringToDealWith;
	std::string::size_type FirstNotSpace = mString.find_first_not_of(' ', 0U);
	if (FirstNotSpace != std::string::npos && FirstNotSpace > 0) {
		mString = mString.substr(FirstNotSpace, mString.length() - FirstNotSpace);
	}
	return mString;
}

std::string EasyCrossPlatform::Parser::StringUtil::rightTrim(const std::string & StringToDealWith)
{
	std::string mString = StringToDealWith;
	std::string::size_type LastNotSpace = mString.find_last_not_of(' ', mString.length());
	if (LastNotSpace != std::string::npos && LastNotSpace < mString.length() - 1) {
		mString = mString.substr(0U, LastNotSpace+1);
	}
	return mString;
}

std::string EasyCrossPlatform::Parser::StringUtil::trim(const std::string & StringToDealWith)
{
	return leftTrim(rightTrim(StringToDealWith));
}

std::string EasyCrossPlatform::Parser::StringUtil::fromBytes(const std::vector<byte>& Bytes)
{
	std::string myStr = "";
	for (size_t i = 0U; i < Bytes.size(); i++) {
		myStr += static_cast<const char>(Bytes[i]);
	}
	return myStr;
}

std::vector<byte> EasyCrossPlatform::Parser::StringUtil::toBytes(const std::string & myStr)
{
	std::vector<byte> mBytes;
	for (size_t i = 0U; i < myStr.length(); i++) {
		mBytes.push_back(static_cast<byte>(myStr[i]));
	}
	return mBytes;
}
