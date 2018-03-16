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

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::analyzeFirstLine(std::string& firstLine)
{
	/*
	Analyze method
	*/
	std::string::size_type FindPos = firstLine.find(' ', 0U);
	if (FindPos != std::string::npos) {
		this->method = EasyCrossPlatform::Parser::StringUtil::toUpper(firstLine.substr(0U, FindPos));
		if (FindPos < firstLine.length() - 1) {
			firstLine = firstLine.substr(FindPos + 1, firstLine.length() - 1 - FindPos);
		}
		else {
			firstLine = "";
		}
	}
	/*
	Analyze Minor
	[/path]/[file][?queryStr][#FragementID] HTTP/major.minor
	*/
	FindPos = firstLine.rfind('.', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			try {
				this->minorVersion = static_cast<unsigned short>(std::stoul(firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1)));
			}
			catch (std::invalid_argument e) {
				//Do Nothing
				this->minorVersion = 0U;
			}
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze Major
	[/path]/[file][?queryStr][#FragementID] HTTP/major
	*/
	FindPos = firstLine.rfind("HTTP/", firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 5) {
			try {
				this->majorVersion = static_cast<unsigned short>(std::stoul(firstLine.substr(FindPos + 5, firstLine.length() - FindPos - 5)));
			}
			catch (std::invalid_argument e) {
				//Do Nothing
				this->majorVersion = 0U;
			}
		}
		
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Get rid of the Space
	[/path]/[file][?queryStr][#FragementID]_
	*/
	if (firstLine.length() >= 1U && firstLine[firstLine.length() - 1] == ' ') {
		firstLine = firstLine.substr(0U, firstLine.length() - 1);
	}
	/*
	Analyze FragementID
	[/path]/[file][?queryStr][#FragementID]
	*/
	FindPos = firstLine.rfind('#', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			this->RequestURL.FragmentID = firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1);
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze QueryStr
	[/path]/[file][?queryStr]
	*/
	FindPos = firstLine.rfind('#', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			this->RequestURL.QueryString = firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1);
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze File
	[/path]/[file]
	*/
	FindPos = firstLine.rfind('/', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			this->RequestURL.FileName = firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1);
		}
		firstLine = firstLine.substr(0U, FindPos);
		/*
		Analyze Path
		[/path]
		*/
		if (!firstLine.empty()) {
			if (firstLine[0] == '/') {
				this->RequestURL.Path = firstLine.substr(1U, firstLine.length() - 1);
			}
			else {
				this->RequestURL.Path = firstLine;
			}
		}
	}
	else {
		this->RequestURL.FileName = firstLine;
	}
}

void EasyCrossPlatform::Parser::HTTP::HTTPRequest::cleanUp()
{
	this->majorVersion = 0U;
	this->minorVersion = 0U;
	this->RequestURL.cleanUp();
	this->method = "";
	this->connection = "";
	this->data = "";
	this->fields.clear();
}

bool EasyCrossPlatform::Parser::HTTP::HTTPRequest::fromReqString(const std::string & Request)
{
	/*
		method [/path]/[file][?queryStr][#FragementID] HTTP/major.minor
		[Header1: HeaderContent1]
		[Header2: HeaderContent2]
		[User-Agent: UserAgent]
		[Host: Host]
		blank line here
		[Post Data]
		
		HTTP1.0 enables clients not specify the host, only after HTTP1.1 clients are able to use Hosts
		If HTTP1.1 Req does not include Host, server should return -> HTTP/1.1 400 Bad Request
	*/
	this->cleanUp(); //Cleanup the data stored in the HTTP Req
	//Split the Request by new lines.
	std::vector<std::pair<std::string::size_type,std::string>> mySeparateStr = EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine(Request, -1);
	/*
		Analyze method, path, file, major, minor
		method [/path]/[file] HTTP/major.minor
	*/
	std::string firstLine = mySeparateStr[0].second;
	//Check if the firstLine is HTTP first
	std::string::size_type HTTPSymbolPos = firstLine.find("HTTP/", 0U);
	if (HTTPSymbolPos == std::string::npos && Request.length() < 2048 && mySeparateStr.size() <= 1) { //Data cannot be too long
		//No information to determine, let it continue feeding data
		return false;
	}
	else if(HTTPSymbolPos == std::string::npos){
		throw std::invalid_argument("The Request is not a proper HTTP request");
		return false;
	}
	this->analyzeFirstLine(firstLine); //Analyze First Line of the Req
	//Analyzing Header Variables until hits an empty string
	auto myIt = mySeparateStr.begin();
	std::string::size_type colonPos = 0U;
	std::string myItValue;
	std::string myItField;
	while (++myIt != mySeparateStr.end() && (!(*myIt).second.empty())) {
		colonPos = (*myIt).second.find(':', 0U);
		if(colonPos!=std::string::npos){
			if (colonPos < (*myIt).second.length() - 1) {
				myItValue = (*myIt).second.substr(colonPos + 1, (*myIt).second.length() - 1 - colonPos);
			}
			myItField = (*myIt).second.substr(0U, colonPos);
			if (myItValue[0] == ' ') {
				myItValue = myItValue.substr(1U, myItValue.length() - 1);
			}
			if (EasyCrossPlatform::Parser::StringUtil::toUpper(myItField) == "HOST") {
				//Host: Host[:Port]
				myItField = "Host";
				std::string::size_type HostSemiColmnPos = myItValue.rfind(':', myItValue.length());
				if (HostSemiColmnPos != std::string::npos) {
					this->RequestURL.Host = myItValue.substr(0U, HostSemiColmnPos);
					if (HostSemiColmnPos < myItValue.length() - 1) {
						try {
							this->RequestURL.Port = static_cast<unsigned short>(std::stoul(myItValue.substr(HostSemiColmnPos + 1, myItValue.length() - HostSemiColmnPos - 1)));
						}
						catch (std::invalid_argument e) {
							//Do Nothing
							this->RequestURL.Port = 0U;
						}
					}
				}
				else {
					this->RequestURL.Host = myItValue;
				}
			}
			else if (EasyCrossPlatform::Parser::StringUtil::toUpper(myItField) == "CONTENT-TYPE") {
				myItField = "Content-Type";
			}
			else if (EasyCrossPlatform::Parser::StringUtil::toUpper(myItField) == "CONTENT-LENGTH") {
				myItField = "Content-Length";
			}
			this->fields[myItField] = myItValue;
		}
	}
	//Finish Analyzing Headers, Stick Bodies together
	if (myIt!=mySeparateStr.end()) {
		if (++myIt != mySeparateStr.end()) {
			std::string::size_type myDataLength;
			if (this->fields.find("Content-Length") != this->fields.end()) {
				try {
					myDataLength = static_cast<std::string::size_type>(std::stoul(this->fields["Content-Length"]));
					if (myDataLength > Request.length() - (*myIt).first) {
						//Return False because no enough data is present
						return false;
					}
				}
				catch (std::invalid_argument e) {
					myDataLength = Request.length() - (*myIt).first;
				}
			}
			else {
				myDataLength = Request.length() - (*myIt).first;
			}
			this->data = Request.substr((*myIt).first, myDataLength);
			return true;
		}
		else {
			if (this->fields.find("Content-Length") == this->fields.end() || this->fields["Content-Length"] == "0") {
				return true;
			}
			else {
				//No Data Present, but Content-Length is set. Return false
				return false;
			}
		}
	}
	else {
		return false;
	}
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPRequest::toReqString()
{
	/*
		method [/path]/[file][?QueryStr] HTTP/major.minor
		[Header1:HeaderContent1]
		[Header2:HeaderContent2]
		[User-Agent:UserAgent]
		[Host:Host]
		blank line here
		[Post Data]
	*/
	//Check if this is successfully parsed

	//Note that FragementID is not needed. And we will not add it anyway.
	std::string myReqString = "";
	std::string myCRLF = "\r\n"; //w3.org Requires ALL HTTP Messages to be separated by rn
	/* 
	Old CRLF:
	std::string myCRLF = EasyCrossPlatform::Parser::StringUtil::newLineString;
	if (myCRLF == "\r") {
		//Just to make sure the system understands.
		myCRLF = "\n";
	}
	*/

	//Adding Method
	myReqString += (this->method.empty() ? std::string("GET") : this->method) + " ";
	//Adding Path
	if (!this->RequestURL.Path.empty()) {
		myReqString += "/" + this->RequestURL.Path;
	}
	//Adding File
	myReqString += "/" + this->RequestURL.FileName;
	//Adding QueryStr
	if (!this->RequestURL.QueryString.empty()) {
		myReqString += "?" + this->RequestURL.QueryString;
	}
	//Adding HTTP/Major.Minor
	myReqString += " HTTP/" + std::to_string(this->majorVersion) + "." + std::to_string(this->minorVersion);
	myReqString += myCRLF;
	//Set Host to Fields
	if (!this->RequestURL.Host.empty() && this->RequestURL.Port == 0U) {
		this->fields["Host"] = this->RequestURL.Host;
	}
	else if (!this->RequestURL.Host.empty() && this->RequestURL.Port != 0U) {
		this->fields["Host"] = this->RequestURL.Host + ":" + std::to_string(this->RequestURL.Port);
	}
	//Set Content-Length to the length of the String
	if (!this->data.empty()) {
		this->fields["Content-Length"] = std::to_string(this->data.length());
	}
	else if(this->fields.find("Content-Length")!=this->fields.end()) {
		this->fields.erase("Content-Length");
	}
	//Starting to add Header Fields
	if (!this->fields.empty()) {
		for(auto myIt = this->fields.begin();myIt!=this->fields.end();myIt++){
			myReqString += (*myIt).first + ": " + (*myIt).second + myCRLF;
		}
	}
	//adding Data
	myReqString += myCRLF;
	if (!this->data.empty()) {
		myReqString += this->data;
	}
	return myReqString;
}

void EasyCrossPlatform::Parser::HTTP::HTTPResponse::cleanUp()
{
	this->statusCode = 0U;
	this->statusDescription = "";
	this->majorVersion = 0U;
	this->minorVersion = 0U;
	this->data = "";
	this->fields.clear();
}

bool EasyCrossPlatform::Parser::HTTP::HTTPResponse::fromResponseString(const std::string & Request)
{
	/*
	HTTP/Major.Minor ResponseCode Description
	[Field1]: [Value1]
	[Content-Type]: [ContentType]
	[Content-Length]: [ContentLength]

	[Data]
	*/
	this->cleanUp();
	std::vector<std::pair<std::string::size_type, std::string>> mySeparateStr = EasyCrossPlatform::Parser::StringUtil::splitStringByNewLine(Request, -1);
	std::string firstLine = mySeparateStr[0].second;
	//Check if this is a correct HTTP Request
	std::string::size_type firstHTTPPos = firstLine.find("HTTP/");
	if (firstHTTPPos != 0U && Request.length() < 200 && mySeparateStr.size() <= 1) {
		//No enough information to determine,Continue
		return false;
	}
	else if(firstHTTPPos != 0U) {
		throw std::invalid_argument("This is not a proper HTTP response");
		return false;
	}
	/*
	Analyze Description First
	FirstLine: HTTP/Major.Minor ResponseCode Description
	*/
	std::string::size_type FindPos = firstLine.rfind(' ', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			this->statusDescription = firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1);
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze Response Code
	FirstLine: HTTP/Major.Minor ResponseCode
	*/
	FindPos = firstLine.rfind(' ', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			try {
				this->statusCode = static_cast<unsigned short>(std::stoul(firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1)));
			}
			catch (std::invalid_argument e) {
				this->statusCode = 0U;
			}
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze Minor
	FirstLine: HTTP/Major.Minor
	*/
	FindPos = firstLine.rfind('.', firstLine.length());
	if (FindPos != std::string::npos) {
		if (FindPos < firstLine.length() - 1) {
			try {
				this->minorVersion = static_cast<unsigned short>(std::stoul(firstLine.substr(FindPos + 1, firstLine.length() - FindPos - 1)));
			}
			catch (std::invalid_argument e) {
				this->minorVersion = 0U;
			}
		}
		firstLine = firstLine.substr(0U, FindPos);
	}
	/*
	Analyze Major
	FirstLine: HTTP/Major
	*/
	if (firstLine.length() > 5) {
		try {
			this->majorVersion = static_cast<unsigned short>(std::stoul(firstLine.substr(5, firstLine.length() - 5)));
		}
		catch (std::invalid_argument e) {
			this->majorVersion = 0U;
		}
	}
	/*
	No longer need first Line, clear it.
	*/
	firstLine.clear();
	/*
	Start Analyzing Fields
	*/
	auto myIt = mySeparateStr.begin();
	std::string::size_type colonPos = 0U;
	std::string myItValue;
	std::string myItField;
	while (++myIt != mySeparateStr.end() && (!(*myIt).second.empty())) {
		colonPos = (*myIt).second.find(':', 0U);
		if (colonPos != std::string::npos) {
			if (colonPos < (*myIt).second.length() - 1) {
				myItValue = (*myIt).second.substr(colonPos + 1, (*myIt).second.length() - 1 - colonPos);
			}
			myItField = (*myIt).second.substr(0U, colonPos);
			if (myItValue[0] == ' ') {
				myItValue = myItValue.substr(1U, myItValue.length() - 1);
			}
			if (EasyCrossPlatform::Parser::StringUtil::toUpper(myItField) == "CONTENT-TYPE") {
				myItField = "Content-Type";
			}
			else if (EasyCrossPlatform::Parser::StringUtil::toUpper(myItField) == "CONTENT-LENGTH") {
				myItField = "Content-Length";
			}
			this->fields[myItField] = myItValue;
		}
	}
	/*
	Finish Analyzing Headers, Reading Content
	*/
	if (myIt != mySeparateStr.end()) {
		if (++myIt != mySeparateStr.end()) {
			std::string::size_type myDataLength;
			if (this->fields.find("Content-Length") != this->fields.end()) {
				try {
					myDataLength = static_cast<std::string::size_type>(std::stoul(this->fields["Content-Length"]));
					if (myDataLength > Request.length() - (*myIt).first) {
						//Return False because no enough data is present
						return false;
					}
				}
				catch (std::invalid_argument e) {
					myDataLength = Request.length() - (*myIt).first;
				}
			}
			else {
				myDataLength = Request.length() - (*myIt).first;
			}
			this->data = Request.substr((*myIt).first, myDataLength);
			return true;
		}
		else {
			if (this->fields.find("Content-Length") == this->fields.end() || this->fields["Content-Length"] == "0") {
				return true;
			}
			else {
				//No Data Present, but Content-Length is set. Return false
				return false;
			}
		}
	}
	else {
		return false;
	}
}

std::string EasyCrossPlatform::Parser::HTTP::HTTPResponse::toResponseString()
{
	/*
	HTTP/Major.Minor ResponseCode Description
	[Field1]: [Value1]
	[Content-Type]: [ContentType]
	[Content-Length]: [ContentLength]

	[Data]
	*/
	std::string myResponseString = "";
	std::string myCRLF = "\r\n";
	//Add HTTP/
	myResponseString += "HTTP/";
	//Add Major.Minor
	myResponseString += std::to_string(this->majorVersion) + "." + std::to_string(this->minorVersion);
	//Add Response Code and Description
	myResponseString += " " + std::to_string(this->statusCode) + " " + this->statusDescription + myCRLF;
	//Set up Content related Fields
	if (!this->data.empty()) {
		this->fields["Content-Length"] = std::to_string(this->data.length());
		if (this->fields.find("Content-Type") == this->fields.end()) {
			this->fields["Content-Type"] = "text/html";
		}
	}
	else {
		if (this->fields.find("Content-Length") != this->fields.end()) {
			this->fields.erase("Content-Length");
		}
	}
	//Dump Fields
	for (auto myIt = this->fields.begin(); myIt != this->fields.end(); myIt++) {
		myResponseString += (*myIt).first + ": " + (*myIt).second + myCRLF;
	}
	//Add Content
	myResponseString += myCRLF;
	if (!this->data.empty()) {
		myResponseString += this->data;
	}
	return myResponseString;
}
