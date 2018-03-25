#include <XSYDHTTPParser.h>

void EasyCrossPlatform::Parser::HTTP::URL::cleanUp()
{
	this->Protocol = "";
	this->AccessCert = "";
	this->Host = "";
	this->Port = 0U;
	this->Path = "";
	this->FileName = "";
	this->QueryString = "";
	this->FragmentID = "";
}

std::string EasyCrossPlatform::Parser::HTTP::URL::toURLString()
{
	/*
	Protocol://[[Access Cert Info@]Host[:Port]][/Path]/[FileName][?QueryString][#Fragment ID]
	*/
	std::string myURL = "";
	if (this->Protocol.empty()) {
		myURL += "http://";
	} else {
		myURL += this->Protocol + "://";
	}
	if ((!this->AccessCert.empty()) || (!this->Host.empty()) || (!this->Port == 0U)) {
		if (!this->AccessCert.empty()) {
			myURL += this->AccessCert + "@";
		}
		if (!this->Host.empty()) {
			myURL += this->Host;
		}
		if (!this->Port == 0U) {
			std::string PortStr = std::to_string(this->Port);
			myURL += ":" + PortStr;
		}
	}
	if (!this->Path.empty()) {
		myURL += "/" + this->Path;
	}
	myURL += "/";
	if (!this->FileName.empty()) {
		myURL += this->FileName;
	}
	if (!this->QueryString.empty()) {
		myURL += "?" + this->QueryString;
	}
	if (!this->FragmentID.empty()) {
		myURL += "#" + this->FragmentID;
	}
	return myURL;
}

void EasyCrossPlatform::Parser::HTTP::URL::fromURLString(const std::string& urlString)
{
	
	this->cleanUp();
	std::string tmpURL = urlString;

	/*
	Protocol Analysis
	[Protocol://][[Access Cert Info@]Host[:Port]][/Path]/[FileName][?QueryString][#Fragment ID]
	*/
	std::string::size_type ProtocolPos = tmpURL.find("://", 0U);
	if (ProtocolPos != std::string::npos) {
		this->Protocol = tmpURL.substr(0U, ProtocolPos);
		tmpURL = tmpURL.substr(ProtocolPos + 3, tmpURL.length() - ProtocolPos - 3);
	}

	/*
	FragementID Analysis
	[[Access Cert Info@]Host[:Port]][/Path]/[FileName][?QueryString][#Fragment ID]
	*/
	std::string::size_type FragmentPos = tmpURL.rfind("#", tmpURL.length());
	if (FragmentPos != std::string::npos) {
		this->FragmentID = tmpURL.substr(FragmentPos + 1, tmpURL.length() - FragmentPos - 1);
		tmpURL = tmpURL.substr(0U, FragmentPos);
	}
	/*
	QueryString Analysis
	[[Access Cert Info@]Host[:Port]][/Path]/[FileName][?QueryString]
	*/
	std::string::size_type QueryPos = tmpURL.rfind("?", tmpURL.length());
	if (QueryPos != std::string::npos) {
		this->QueryString = tmpURL.substr(QueryPos + 1, tmpURL.length() - QueryPos - 1);
		tmpURL = tmpURL.substr(0U, QueryPos);
	}
	/*
	Get [[Access Cert Info@]Host[:Port]] Part
	[[Access Cert Info@]Host[:Port]][/Path]/[FileName]
	*/
	std::string::size_type FirstSlashPos = tmpURL.find("/", 0);
	if (FirstSlashPos != std::string::npos) {
		std::string AccessCertHostPortStr = tmpURL.substr(0U, FirstSlashPos);
		tmpURL = tmpURL.substr(FirstSlashPos + 1, tmpURL.length() - FirstSlashPos - 1);
		/*
		Analyze Access Cert Info
		AccessCertHostPortStr = [Access Cert Info@]Host[:Port]
		*/
		std::string::size_type AccessPos = AccessCertHostPortStr.find("@", 0);
		if (AccessPos != std::string::npos) {
			this->AccessCert = AccessCertHostPortStr.substr(0U, AccessPos);
			AccessCertHostPortStr = AccessCertHostPortStr.substr(AccessPos + 1, AccessCertHostPortStr.length() - AccessPos - 1);
		}
		/*
		Analyze Port Info
		AccessCertHostPortStr = Host[:Port]
		*/
		std::string::size_type PortPos = AccessCertHostPortStr.rfind(":", AccessCertHostPortStr.length());
		if (PortPos != std::string::npos) {
			try {
				this->Port = static_cast<unsigned short>(std::stoul(AccessCertHostPortStr.substr(PortPos + 1, AccessCertHostPortStr.length() - PortPos - 1), 0, 10));
			}
			catch (std::invalid_argument e) {
				this->Port = 0U;
			}
			AccessCertHostPortStr = AccessCertHostPortStr.substr(0U, PortPos);
		}
		/*
		Analyze Host Info
		AccessCertHostPortStr = Host
		*/
		this->Host = AccessCertHostPortStr;
	}
	else {
		this->Host = tmpURL;
		tmpURL = "";
	}
	/*
	Analyze Filename
	[Path]/[FileName] or [FileName]
	*/
	std::string::size_type FilePos = tmpURL.rfind("/", tmpURL.length());
	if (FilePos != std::string::npos) {
		//Has [Path]/[FileName]
		this->FileName = tmpURL.substr(FilePos + 1, tmpURL.length() - FilePos - 1);
		tmpURL = tmpURL.substr(0U, FilePos);
		/*
		Analyze Path
		[Path]
		*/
		this->Path = tmpURL;
		return;
	}
	else {
		//Only Has [FileName]
		this->FileName = tmpURL;
		return;
	}
}

void EasyCrossPlatform::Parser::HTTP::URL::setAccessCert(const std::string & Username, const std::string & Password)
{
	if (Username.empty() && Password.empty()) {
		this->AccessCert = "";
		return;
	}
	if (Password.empty()) {
		this->AccessCert = Username;
		return;
	} else {
		this->AccessCert = Username + ":" + Password;
		return;
	}
}

std::pair<std::string, std::string> EasyCrossPlatform::Parser::HTTP::URL::getAccessCert()
{
	std::string Username = "", Password = "";
	std::string tmpAccess = this->AccessCert;
	/*
	Analyze Password
	[Username][:Password]
	*/
	std::string::size_type PassPos = tmpAccess.find(":",0);
	if (PassPos != std::string::npos) {
		Password = tmpAccess.substr(PassPos + 1, tmpAccess.length() - PassPos - 1);
		tmpAccess = tmpAccess.substr(0U, PassPos);
	}
	/*
	Analyze Username
	[Username]
	*/
	Username = tmpAccess;
	return std::pair<std::string, std::string>(Username,Password);
}

bool EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::ParseFirstLine(const std::string & FirstLine)
{
	/*
		[METHOD] [/path]/[file][?QueryString][#FragementID] HTTP/Major.Minor
	*/
	std::string::size_type FirstPos = 0U;
	std::string mFirstLine = FirstLine;
	FirstPos = mFirstLine.find(' ', 0U);
	if (FirstPos != std::string::npos && FirstPos < mFirstLine.size() - 1U) {
		this->Method = mFirstLine.substr(0U, FirstPos);
		mFirstLine = mFirstLine.substr(FirstPos + 1U, mFirstLine.size() - FirstPos - 1U);
	}
	else {
		return false;
	}
	/*
		[/path]/[file][?QueryString][#FragmentID] HTTP/Major.Minor
	*/
	FirstPos = mFirstLine.find(' ', 0U);
	if (FirstPos != std::string::npos && FirstPos < mFirstLine.size() - 1U) {
		std::string AccessPos = mFirstLine.substr(0U, FirstPos);
		mFirstLine = mFirstLine.substr(FirstPos + 1U, mFirstLine.size() - FirstPos - 1U);

		this->RequestedURL.fromURLString(AccessPos);
		/*
		//[/path]/[file][?QueryString][#FragmentID]
		FirstPos = AccessPos.rfind('#', AccessPos.size());
		if (FirstPos != std::string::npos) {
			if (FirstPos < AccessPos.size() - 1U) {
				this->RequestedURL.FragmentID = AccessPos.substr(FirstPos + 1U, AccessPos.size() - FirstPos - 1U);
			}
			AccessPos = AccessPos.substr(0U, FirstPos);
		}
		//[/path]/[file][?QueryString]
		FirstPos = AccessPos.rfind('?', AccessPos.size());
		if (FirstPos != std::string::npos) {
			if (FirstPos < AccessPos.size() - 1U) {
				this->RequestedURL.QueryString = AccessPos.substr(FirstPos + 1U, AccessPos.size() - FirstPos - 1U);
			}
			AccessPos = AccessPos.substr(0U, FirstPos);
		}
		//[/path]/[file]
		FirstPos = AccessPos.rfind('/', AccessPos.size());
		if (FirstPos != std::string::npos) {
			if (FirstPos < AccessPos.size() - 1U) {
				this->RequestedURL.FileName = AccessPos.substr(FirstPos + 1U, AccessPos.size() - FirstPos - 1U);
			}
			AccessPos = AccessPos.substr(0U, FirstPos);
		}
		if ((AccessPos.size() > 1U) && AccessPos[0] == '/') {
			this->RequestedURL.Path = AccessPos.substr(1U, AccessPos.size() - 1U);
		}
		else if((!AccessPos.empty()) && AccessPos[0] != '/'){
			this->RequestedURL.Path = AccessPos;
		}
		*/
	}
	else {
		return false;
	}
	/*
	HTTP/Major.Minor
	*/
	if (mFirstLine.find("HTTP/") == 0U && mFirstLine.size() > 5U) {
		mFirstLine = mFirstLine.substr(5U, mFirstLine.size() - 5U);
		FirstPos = mFirstLine.find('.', 0U);
		if (FirstPos != std::string::npos) {
			try {
				this->MajorVersion = std::stoul(mFirstLine.substr(0U, FirstPos));
			}
			catch (std::invalid_argument e) {
				this->MajorVersion = 0U;
				return false;
			}
		}
		else {
			return false;
		}
		if (FirstPos != std::string::npos && FirstPos < mFirstLine.size() - 1U) {
			try {
				this->MinorVersion = std::stoul(mFirstLine.substr(FirstPos + 1U, mFirstLine.size() - FirstPos - 1U));
			}
			catch (std::invalid_argument e) {
				this->MinorVersion = 0U;
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	return true;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::WriteFirstLine()
{
	/*
	[METHOD] [/path]/[file][?QueryString][#FragementID] HTTP/Major.Minor
	*/
	std::string mFirstLine = "";
	mFirstLine += this->Method;
	mFirstLine += ' ';
	if (!this->RequestedURL.Path.empty()) {
		mFirstLine += '/';
		mFirstLine += this->RequestedURL.Path;
	}
	mFirstLine += '/';
	mFirstLine += this->RequestedURL.FileName;
	if (!this->RequestedURL.QueryString.empty()) {
		mFirstLine += '?';
		mFirstLine += this->RequestedURL.QueryString;
	}
	mFirstLine += ' ';
	mFirstLine += "HTTP/";
	mFirstLine += std::to_string(this->MajorVersion);
	mFirstLine += '.';
	mFirstLine += std::to_string(this->MinorVersion);
	return mFirstLine;
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::AnalyzeField(const std::string & SingleLine)
{
	/*
	Field: Value
	*/
	std::string::size_type FirstPos = SingleLine.find(':', 0U);
	if (FirstPos == std::string::npos) {
		return;
	}
	std::string FieldName = SingleLine.substr(0U, FirstPos);
	std::string newFieldName = "";
	std::string FieldValue = "";
	if (FirstPos < SingleLine.size() - 1U) {
		FieldValue = SingleLine.substr(FirstPos + 1U, SingleLine.size() - FirstPos - 1U);
	}
	FieldValue = EasyCrossPlatform::Parser::StringUtil::leftTrim(FieldValue);
	std::vector<std::pair<std::string::size_type, std::string>> mSeparatedFieldName = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(FieldName, "-", -1);
	for (unsigned int i = 0; i < mSeparatedFieldName.size(); i++) {
		newFieldName += EasyCrossPlatform::Parser::StringUtil::toSentence(mSeparatedFieldName[i].second) + "-";
	}
	newFieldName = newFieldName.substr(0U, newFieldName.size() - 1U);
	this->FieldsValues[newFieldName] = FieldValue;
	if (newFieldName == "Host") {
		this->RequestedURL.Host = FieldValue;
	}
	return;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::WriteField(const std::string & FieldName)
{
	//Field: Value
	std::string newFieldName = "";
	std::vector<std::pair<std::string::size_type, std::string>> mSeparatedFieldName = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(FieldName, "-", -1);
	for (unsigned int i = 0; i < mSeparatedFieldName.size(); i++) {
		newFieldName += EasyCrossPlatform::Parser::StringUtil::toSentence(mSeparatedFieldName[i].second) + "-";
	}
	newFieldName = newFieldName.substr(0U, newFieldName.size() - 1U);
	std::string mResult = newFieldName + ": " + this->FieldsValues[FieldName];
	return mResult;
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::cleanUp()
{
	this->MajorVersion = 0U;
	this->MinorVersion = 0U;
	this->Method = "";
	this->RequestedURL.cleanUp();
	this->FieldsValues.clear();
}

EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::fromReqString(const std::string & ReqString)
{
	this->cleanUp();

	HTTPParseReturnVal mReturnVal;
	std::vector<std::pair<std::string::size_type, std::string>> mSeparateLines = EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine(ReqString, -1);
	
	std::string myFirstLine = mSeparateLines[0].second;
	mReturnVal.msgIsHTTP = this->ParseFirstLine(myFirstLine);
	if (!mReturnVal.msgIsHTTP) {
		if (mSeparateLines.size() <= 1U && ReqString.size() < MaxReqAnalyzeSize) { //Not Big Enough for Analyze if the Msg is HTTP
			mReturnVal.msgIsEnough = false;
			mReturnVal.msgIsHTTP = false;
			mReturnVal.canDecode = false;
			mReturnVal.onError = false;
			mReturnVal.RemainMsg = ReqString;
		}
		else { //Msg is big enough for analyze if the msg is http, but it is not HTTP.
			mReturnVal.msgIsEnough = true;
			mReturnVal.msgIsHTTP = false;
			mReturnVal.canDecode = false;
			mReturnVal.onError = false;
			mReturnVal.RemainMsg = ReqString;
		}
		return mReturnVal;
	}
	size_t mCurrentLine = 1U;
	std::string CurrentLineData = "";
	for (; mCurrentLine < mSeparateLines.size(); mCurrentLine++) {
		CurrentLineData = mSeparateLines[mCurrentLine].second;
		this->AnalyzeField(CurrentLineData);
		//Waiting Until Hits an Empty String
		if (CurrentLineData.empty()) {
			break;
		}
	}
	if (mCurrentLine < mSeparateLines.size()) {
		mCurrentLine++;
		if (mCurrentLine == mSeparateLines.size()) {
			mReturnVal.msgIsEnough = false;
			mReturnVal.msgIsHTTP = true;
			mReturnVal.canDecode = true;
			mReturnVal.onError = false;
			mReturnVal.RemainMsg = ReqString;
			return mReturnVal;
		}
		std::string::size_type DataStartPlace = mSeparateLines[mCurrentLine].first;
		mReturnVal.msgIsEnough = true;
		mReturnVal.msgIsHTTP = true;
		mReturnVal.canDecode = true;
		mReturnVal.onError = false;
		mReturnVal.RemainMsg = ReqString.substr(DataStartPlace, ReqString.size() - DataStartPlace);
		return mReturnVal;
	}
	else {
		mReturnVal.msgIsEnough = false;
		mReturnVal.msgIsHTTP = true;
		mReturnVal.canDecode = true;
		mReturnVal.onError = false;
		mReturnVal.RemainMsg = ReqString;
		return mReturnVal;
	}
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequestHeader::toReqString()
{
	std::string newLineStr = "\r\n";
	std::string mResult = "";
	mResult += this->WriteFirstLine() + newLineStr;
	if (!this->RequestedURL.Host.empty()) {
		if (this->RequestedURL.Port != 0U) {
			this->FieldsValues["Host"] = this->RequestedURL.Host + ":" + std::to_string(this->RequestedURL.Port);
		}
		else {
			this->FieldsValues["Host"] = this->RequestedURL.Host;
		}
	}
	for (auto i = this->FieldsValues.begin(); i != this->FieldsValues.end(); i++) {
		mResult += this->WriteField((*i).first) + newLineStr;
	}
	mResult += newLineStr;
	return mResult;
}

EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal EasyCrossPlatform::Parser::HTTP::HTTPRequest::parseHeader(const std::string & ReqString)
{
	HTTPParseReturnVal mResult;
	HTTPRequestHeader mHeaderCls;
	HTTPParseReturnVal mHeaderResult = mHeaderCls.fromReqString(ReqString);
	if (!mHeaderResult.msgIsEnough || !mHeaderResult.msgIsHTTP || !mHeaderResult.canDecode || mHeaderResult.onError) {
		mResult = mHeaderResult;
		return mResult;
	}
	this->FieldsValues = mHeaderCls.FieldsValues;
	this->MajorVersion = mHeaderCls.MajorVersion;
	this->MinorVersion = mHeaderCls.MinorVersion;
	this->RequestedURL = mHeaderCls.RequestedURL;
	this->Method = mHeaderCls.Method;
	if (this->FieldsValues.find("Accept-Encoding") != this->FieldsValues.end()) {
		std::string AcceptEncodingOriginStr = this->FieldsValues["Accept-Encoding"];
		this->AnalyzeAcceptEncodingValue(AcceptEncodingOriginStr);
	}
	if (this->FieldsValues.find("Content-Encoding") != this->FieldsValues.end()) {
		std::string ContentEncodingOriginStr = this->FieldsValues["Content-Encoding"];
		this->AnalyzeContentEncodingValue(ContentEncodingOriginStr);
	}
	if (this->FieldsValues.find("Transfer-Encoding") != this->FieldsValues.end()) {
		std::string TransferEncodingOriginStr = this->FieldsValues["Transfer-Encoding"];
		this->AnalyzeTransferEncodingValue(TransferEncodingOriginStr);
	}
	if (this->FieldsValues.find("Connection") != this->FieldsValues.end()) {
		this->Connection = this->FieldsValues["Connection"];
	}
	mResult.canDecode = true;
	mResult.msgIsEnough = true;
	mResult.msgIsHTTP = true;
	mResult.RemainMsg = mHeaderResult.RemainMsg;
	return mResult;
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::AnalyzeAcceptEncodingValue(const std::string & AcceptEncodingVal)
{
	if (AcceptEncodingVal.empty()) {
		return;
	}
	std::vector<std::pair<std::string::size_type, std::string>> mSplitEncoding = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(AcceptEncodingVal, ",", -1);
	std::string SingleEncoding = "";
	std::string::size_type LastPos = 0U;
	float tempWeight = 0.0F;
	for (auto i = mSplitEncoding.begin(); i != mSplitEncoding.end(); i++) {
		SingleEncoding = (*i).second;
		SingleEncoding = EasyCrossPlatform::Parser::StringUtil::trim(SingleEncoding);
		if (SingleEncoding.empty()) {
			continue;
		}
		LastPos = SingleEncoding.find(";q=");
		if (LastPos == std::string::npos) {
			this->AcceptEncoding.push_back(std::pair<std::string, float>(EasyCrossPlatform::Parser::StringUtil::toLower(SingleEncoding), 0.8F));
		}
		else {
			if (LastPos < SingleEncoding.size() - 3U) {
				try {
					tempWeight = std::stof(SingleEncoding.substr(LastPos + 3U, SingleEncoding.size() - LastPos - 3U));
				}
				catch (std::invalid_argument e) {
					tempWeight = 0.8F;
				}
			}
			else {
				tempWeight = 0.8F;
			}
			this->AcceptEncoding.push_back(std::pair<std::string, float>(EasyCrossPlatform::Parser::StringUtil::toLower(SingleEncoding.substr(0U, LastPos)), tempWeight));
		}
	}
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::AnalyzeContentEncodingValue(const std::string & ContentEncodingVal)
{
	if (ContentEncodingVal.empty()) {
		return;
	}
	std::vector<std::pair<std::string::size_type, std::string>> mSplitEncoding = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(ContentEncodingVal, ",", -1);
	std::string SingleEncoding = "";
	std::string::size_type LastPos = 0U;
	float tempWeight = 0.0F;
	for (auto i = mSplitEncoding.begin(); i != mSplitEncoding.end(); i++) {
		SingleEncoding = (*i).second;
		SingleEncoding = EasyCrossPlatform::Parser::StringUtil::trim(SingleEncoding);
		if (SingleEncoding.empty()) {
			continue;
		}
		this->ContentEncoding.push_back(EasyCrossPlatform::Parser::StringUtil::toLower(SingleEncoding));
	}
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::AnalyzeTransferEncodingValue(const std::string & TransferEncodingVal)
{
	if (TransferEncodingVal.empty()) {
		return;
	}
	std::vector<std::pair<std::string::size_type, std::string>> mSplitEncoding = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(TransferEncodingVal, ",", -1);
	std::string SingleEncoding = "";
	std::string::size_type LastPos = 0U;
	float tempWeight = 0.0F;
	for (auto i = mSplitEncoding.begin(); i != mSplitEncoding.end(); i++) {
		SingleEncoding = (*i).second;
		SingleEncoding = EasyCrossPlatform::Parser::StringUtil::trim(SingleEncoding);
		if (SingleEncoding.empty()) {
			continue;
		}
		this->TransferEncoding.push_back(EasyCrossPlatform::Parser::StringUtil::toLower(SingleEncoding));
	}
}

EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal EasyCrossPlatform::Parser::HTTP::HTTPRequest::DecodeData()
{
	HTTPParseReturnVal mResult;
	mResult.msgIsHTTP = true;
	std::string::size_type MsgLength = 0U;
	std::string ActualMsgNeedToDecode = "";
	std::string ActualRemainingMsg = "";
	if (this->FieldsValues.find("Content-Length") != this->FieldsValues.end()) {
		try {
			MsgLength = static_cast<std::string::size_type>(std::stoul(this->FieldsValues["Content-Length"]));
		}
		catch (std::invalid_argument e) {
			MsgLength = std::string::npos;
		}
	}
	else {
		MsgLength = 0U;
	}
	if (MsgLength == 0U) {
		if (this->TransferEncoding.empty()) {
			mResult.canDecode = true;
			mResult.msgIsEnough = true;
			mResult.RemainMsg = this->EncodedData;
			mResult.onError = false;
			return mResult;
		}
		else if(this->TransferEncoding[this->TransferEncoding.size() -1U] != "chuncked") {
			mResult.canDecode = true;
			mResult.msgIsEnough = true;
			mResult.RemainMsg = this->EncodedData;
			mResult.onError = false;
			return mResult;
		}
		else if (this->TransferEncoding[this->TransferEncoding.size() - 1U] == "chuncked") {
			ActualMsgNeedToDecode = this->EncodedData;
		}
	}
	else if (MsgLength == std::string::npos) {
		mResult.canDecode = false;
		mResult.msgIsEnough = true;
		mResult.msgIsHTTP = true;
		mResult.onError = true;
		mResult.RemainMsg = this->EncodedData;
		return mResult;
	}
	else if (MsgLength > this->EncodedData.size()) {
		mResult.canDecode = true;
		mResult.msgIsEnough = false;
		mResult.msgIsHTTP = true;
		mResult.onError = false;
		mResult.RemainMsg = this->EncodedData;
		return mResult;
	}else{ //MsgLength <= this->EncodedData.size()
		ActualMsgNeedToDecode = this->EncodedData.substr(0U, MsgLength);
		ActualRemainingMsg = this->EncodedData.substr(MsgLength, this->EncodedData.size() - MsgLength);
	}
	std::vector<std::string> MethodToProceed;
	for (auto i = this->TransferEncoding.end() - 1; i >= this->TransferEncoding.begin(); i--) {
		MethodToProceed.push_back((*i));
	}
	for (auto i = this->ContentEncoding.end() - 1; i >= this->ContentEncoding.begin(); i--) {
		MethodToProceed.push_back((*i));
	}
	std::string TmpEncodedData = ActualMsgNeedToDecode;
	std::string TmpDecodeData = "";
	for (auto i = MethodToProceed.begin(); i != MethodToProceed.end(); i++) {
		if ((*i) == "chuncked") {
			HTTPParseReturnVal mChunckedResult = DecodeChunckedData(TmpEncodedData, TmpDecodeData);
			if (!mChunckedResult.msgIsEnough || !mChunckedResult.canDecode || !mChunckedResult.msgIsHTTP || mChunckedResult.onError) {
				mResult = mChunckedResult;
				mResult.RemainMsg = this->EncodedData;
				return mResult;
			}
			ActualRemainingMsg = mChunckedResult.RemainMsg + ActualRemainingMsg;
			TmpEncodedData = TmpDecodeData;
		}
		else if ((*i) == "deflate") {
			TmpDecodeData = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::Deflate::Decrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpEncodedData)));
			TmpEncodedData = TmpDecodeData;
		}
		else if ((*i) == "gzip") {
			TmpDecodeData = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::GZip::Decrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpEncodedData)));
			TmpEncodedData = TmpDecodeData;
		}
		else if ((*i) == "br") {
			TmpDecodeData = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::Brotli::Decrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpEncodedData)));
			TmpEncodedData = TmpDecodeData;
		}
		else if ((*i) == "identity") {
			TmpDecodeData = TmpEncodedData;
		}
	}
	mResult.canDecode = true;
	mResult.msgIsEnough = true;
	mResult.msgIsHTTP = true;
	mResult.onError = false;
	mResult.RemainMsg = ActualRemainingMsg;
	this->OriginalData = TmpDecodeData;
	return mResult;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::WriteHeader()
{
	HTTPRequestHeader mReqHeaderCls;
	if (!this->AcceptEncoding.empty()) {
		this->FieldsValues["Accept-Encoding"] = this->WriteAcceptEncodingValue();
	}
	if (!this->ContentEncoding.empty()) {
		this->FieldsValues["Content-Encoding"] = this->WriteContentEncodingValue();
	}
	if (!this->TransferEncoding.empty()) {
		this->FieldsValues["Tranfer-Encoding"] = this->WriteTransferEncodingValue();
	}
	if (!this->Connection.empty()) {
		this->FieldsValues["Connection"] = this->Connection;
	}
	if (this->TransferEncoding.empty() || (!this->TransferEncoding.empty() && this->TransferEncoding[this->TransferEncoding.size()-1U] != "chuncked")) {
		if (!this->OriginalData.empty()) {
			this->EncodeData();
		}
		this->FieldsValues["Content-Length"] = std::to_string(this->EncodedData.size());
	}
	mReqHeaderCls.FieldsValues = this->FieldsValues;
	mReqHeaderCls.MajorVersion = this->MajorVersion;
	mReqHeaderCls.MinorVersion = this->MinorVersion;
	mReqHeaderCls.Method = this->Method;
	mReqHeaderCls.RequestedURL = this->RequestedURL;
	return mReqHeaderCls.toReqString();
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::toReqString()
{
	std::string mResult;
	mResult += this->WriteHeader(); //WriteHeader will automatically call EncodeData() method, no need to call twice.
	mResult += this->EncodedData;
	return mResult;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::WriteAcceptEncodingValue()
{
	/* Accept-Encoding: Method1;q=0.8, Method2;q=1.0*/
	std::string mResult = "";
	if (!this->AcceptEncoding.empty()) {
		for (auto i = this->AcceptEncoding.begin(); i != this->AcceptEncoding.end(); i++) {
			mResult += (*i).first + ";q=" + std::to_string((*i).second) + ", ";
		}
		mResult = mResult.substr(0U, mResult.size() - 2U);
	}
	return mResult;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::WriteContentEncodingValue()
{
	std::string mResult = "";
	if (!this->ContentEncoding.empty()) {
		for (auto i = this->ContentEncoding.begin(); i != this->ContentEncoding.end(); i++) {
			mResult += (*i) + ", ";
		}
		mResult = mResult.substr(0U, mResult.size() - 2U);
	}
	return mResult;
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::WriteTransferEncodingValue()
{
	std::string mResult = "";
	if (!this->TransferEncoding.empty()) {
		for (auto i = this->TransferEncoding.begin(); i != this->TransferEncoding.end(); i++) {
			mResult += (*i) + ", ";
		}
		mResult = mResult.substr(0U, mResult.size() - 2U);
	}
	return mResult;
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::EncodeData()
{
	std::string newLineStr = "\r\n";
	std::string mEncodedData;
	std::vector<std::string> EncodingMethodLists;
	for (auto i = this->ContentEncoding.begin(); i != this->ContentEncoding.end(); i++) {
		EncodingMethodLists.push_back((*i));
	}
	for (auto i = this->TransferEncoding.begin(); i != this->TransferEncoding.end(); i++) {
		EncodingMethodLists.push_back((*i));
	}
	std::string TmpEncodedString = "";
	std::string TmpOriginalString = this->OriginalData;
	for (auto i = EncodingMethodLists.begin(); i != EncodingMethodLists.end(); i++) {
		if ((*i) == "chuncked") {
			std::string::size_type NumRead = 0U;
			std::string TempHexString = "";
			std::string RemainingStr = TmpOriginalString;
			while (RemainingStr.length() >= ChunckedSplitSize) {
				TmpEncodedString += FromDecToHexString(ChunckedSplitSize) + newLineStr;
				TmpEncodedString += RemainingStr.substr(0U, ChunckedSplitSize) + newLineStr;
				RemainingStr = RemainingStr.substr(ChunckedSplitSize, RemainingStr.size() - ChunckedSplitSize);
			}
			TmpEncodedString += FromDecToHexString(RemainingStr.size()) + newLineStr;
			TmpEncodedString += RemainingStr + newLineStr;
			RemainingStr = "";
			TmpEncodedString += "0" + newLineStr;
			TmpEncodedString += newLineStr;
			TmpOriginalString = TmpEncodedString;
		}
		else if ((*i) == "deflate") {
			TmpEncodedString = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::Deflate::Encrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpOriginalString), this->CompressionLevel));
			TmpOriginalString = TmpEncodedString;
		}
		else if ((*i) == "gzip") {
			TmpEncodedString = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::GZip::Encrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpOriginalString), this->CompressionLevel));
			TmpOriginalString = TmpEncodedString;
		}
		else if ((*i) == "br") {
			TmpEncodedString = EasyCrossPlatform::Parser::StringUtil::fromBytes(EasyCrossPlatform::Compression::Brotli::Encrypt(EasyCrossPlatform::Parser::StringUtil::toBytes(TmpOriginalString), this->CompressionLevel));
			TmpOriginalString = TmpEncodedString;
		}
		else if ((*i) == "identity") {
			TmpEncodedString = TmpOriginalString;
		}
		else {
			TmpEncodedString = TmpOriginalString;
		}
	}
	this->EncodedData = TmpEncodedString;
	return;
}

EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal EasyCrossPlatform::Parser::HTTP::HTTPRequest::fromReqString(const std::string & ReqString)
{
	this->cleanUp();
	HTTPParseReturnVal mResult;
	HTTPParseReturnVal mParseHeaderResult = this->parseHeader(ReqString);
	if (!mParseHeaderResult.canDecode || !mParseHeaderResult.msgIsEnough || !mParseHeaderResult.msgIsHTTP || mParseHeaderResult.onError){
		mResult = mParseHeaderResult;
		mResult.RemainMsg = ReqString;
		return mResult;
	}
	std::string mEncodedData = mParseHeaderResult.RemainMsg;
	this->EncodedData = mEncodedData;
	HTTPParseReturnVal mParseBodyResult = this->DecodeData();
	if (!mParseBodyResult.canDecode || !mParseBodyResult.msgIsEnough || !mParseBodyResult.msgIsHTTP || mParseBodyResult.onError) {
		mResult = mParseBodyResult;
		mResult.RemainMsg = ReqString;
		return mResult;
	}
	mResult.canDecode = true;
	mResult.msgIsEnough = true;
	mResult.msgIsHTTP = true;
	mResult.onError = false;
	mResult.RemainMsg = mParseBodyResult.RemainMsg;
	return mResult;
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::cleanUp()
{
	this->MajorVersion = 0U;
	this->MinorVersion = 0U;
	this->AcceptEncoding.clear();
	this->ContentEncoding.clear();
	this->EncodedData = "";
	this->OriginalData = "";
	this->Connection = "";
	this->Method = "";
	this->FieldsValues.clear();
	this->RequestedURL.cleanUp();
}

unsigned int EasyCrossPlatform::Parser::HTTP::FromHexStringToDec(const std::string & HexString)
{
	//Unsigned int range from 0 ~ 4294967295
	std::string mHexString = EasyCrossPlatform::Parser::StringUtil::toUpper(HexString);
	unsigned int CurrentNum = 0U;
	unsigned int TotalNum = 0U;
	char tmpChar = ' ';
	for (unsigned int i = 0; i < HexString.size(); i++) {
		tmpChar = HexString[HexString.size() - 1U - i];
		if (tmpChar == 'A') {
			CurrentNum = 10U;
		}
		else if (tmpChar == 'B') {
			CurrentNum = 11U;
		}
		else if (tmpChar == 'C') {
			CurrentNum = 12U;
		}
		else if (tmpChar == 'D') {
			CurrentNum = 13U;
		}
		else if (tmpChar == 'E') {
			CurrentNum = 14U;
		}
		else if (tmpChar == 'F') {
			CurrentNum = 15U;
		}
		else if(tmpChar>='0' && tmpChar <= '9') {
			CurrentNum = static_cast<unsigned int>((int) (tmpChar - '0'));
		}
		else {
			return 0U;
		}
		TotalNum += (CurrentNum * (16 ^ i));
	}
	return TotalNum;
}

std::string EasyCrossPlatform::Parser::HTTP::FromDecToHexString(const unsigned int Num)
{
	//Unsigned int range from 0 ~ 4294967295
	//Divide from Power 8 of 16
	unsigned int TempNum = 0U;
	unsigned int RemainNum = Num;
	std::string TempStr;
	for (unsigned int i = 8U; i >= 0; i--) {
		TempNum = RemainNum / (16 ^ i);
		RemainNum -= (TempNum * (16 ^ i));
		if (TempNum >= 1U && TempNum <= 9U) {
			TempStr += (char) (TempNum + '0');
		}
		else if(TempNum == 10U) {
			TempStr += 'A';
		}
		else if (TempNum == 11U) {
			TempStr += 'B';
		}
		else if (TempNum == 12U) {
			TempStr += 'C';
		}
		else if (TempNum == 13U) {
			TempStr += 'D';
		}
		else if (TempNum == 14U) {
			TempStr += 'E';
		}
		else if (TempNum == 15U) {
			TempStr += 'F';
		}
	}
	return TempStr;
}

EasyCrossPlatform::Parser::HTTP::HTTPParseReturnVal EasyCrossPlatform::Parser::HTTP::DecodeChunckedData(const std::string & EncryptedData, std::string & DataForWriting)
{
	HTTPParseReturnVal mReturnVal;
	mReturnVal.msgIsHTTP = true;
	std::string mTempHexNum;
	std::string newLineStr = "\r\n";

	std::string::size_type EndingPlace = std::string::npos;
	std::string mTempContent;
	std::string OverallContent;
	std::vector<std::pair<std::string::size_type, std::string>> mSeparatedStrs = EasyCrossPlatform::Parser::StringUtil::splitStringByDivisor(EncryptedData, newLineStr, -1);
	std::string::size_type NumContent;

	unsigned int mCurrentLine = 0U;
	unsigned int NumReaded = 0U;
	for(;mCurrentLine<mSeparatedStrs.size();mCurrentLine++){
		mTempHexNum = mSeparatedStrs[mCurrentLine].second;
		NumContent = FromHexStringToDec(mTempHexNum);
		if (NumContent == 0U) {
			EndingPlace = mSeparatedStrs[mCurrentLine].first + 2*newLineStr.size();
			break;
		}
		mCurrentLine++;
		if (mSeparatedStrs.size() == mCurrentLine) {
			EndingPlace = std::string::npos;
			break;
		}
		else if(EncryptedData.size() - mSeparatedStrs[mCurrentLine].first < NumContent) {
			EndingPlace = std::string::npos;
			break;
		}
		NumReaded = 0U;
		while (NumReaded < NumContent) {
			mTempContent += mSeparatedStrs[mCurrentLine].second;
			NumReaded += mSeparatedStrs[mCurrentLine].second.size();
			if (NumReaded < NumContent) {
				mTempContent += newLineStr;
				NumReaded += newLineStr.size();
				mCurrentLine++;
			}
		}
		if (NumReaded > NumContent) {
			EndingPlace = std::string::npos;
			break;
		}
		OverallContent += mTempContent;
	}
	if (mCurrentLine == mSeparatedStrs.size() && EndingPlace==std::string::npos) {
		mReturnVal.msgIsHTTP = true;
		mReturnVal.canDecode = true;
		mReturnVal.msgIsEnough = false;
		mReturnVal.onError = false;
		mReturnVal.RemainMsg = EncryptedData;
	}
	if (mCurrentLine < mSeparatedStrs.size() && EndingPlace == std::string::npos) {
		mReturnVal.msgIsHTTP = true;
		mReturnVal.canDecode = false;
		mReturnVal.msgIsEnough = true;
		mReturnVal.onError = true;
		mReturnVal.RemainMsg = EncryptedData;
	}
	else{ //EndingPlace != std::string::npos
		mReturnVal.msgIsHTTP = true;
		mReturnVal.canDecode = true;
		mReturnVal.msgIsEnough = true;
		mReturnVal.onError = false;
		if (EndingPlace < EncryptedData.size()) {
			mReturnVal.RemainMsg = EncryptedData.substr(EndingPlace, EncryptedData.size() - EndingPlace);
		}
		else {
			mReturnVal.RemainMsg = "";
		}
		DataForWriting = OverallContent;
	}
	return mReturnVal;
}