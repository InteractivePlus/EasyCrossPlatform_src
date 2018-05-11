#include <XSYDHTTPDownload.h>

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformRequest(const RequestMethod mRequestMethod, const std::string& URL, const std::string& PostData, const std::pair<std::string, std::string>& AuthData, const unsigned int RecursionTime)
{
	WebsiteRequest mRequestCls;
	if (RecursionTime >= Request::MaxRedirectTime) {
		this->DownloadSucceed = false;
		return;
	}
	mRequestCls.cleanRequest();
	mRequestCls.cleanUp();
	this->cleanUp();
	switch (mRequestMethod) {
	case RequestMethod::GET:
	case RequestMethod::DEL:
		mRequestCls.Method = mRequestMethod;
		break;
	case RequestMethod::POST:
	case RequestMethod::PUT:
	case RequestMethod::TRACE:
	case RequestMethod::HEAD:
		mRequestCls.Method = mRequestMethod;
		mRequestCls.RequestData = PostData;
		break;
	
	default:
		this->DownloadSucceed = false;
		return;
		break;
	}
	mRequestCls.RequestURL = URL;
	mRequestCls.m_ResquestContent.SetFieldWithSingleValue("Accept-Language", this->AcceptLanguageString);
	if ((!AuthData.first.empty()) || (!AuthData.second.empty())) {
		std::string EncodingAuthText = AuthData.first + ":" + AuthData.second;
		std::string EncodedAuthText = EasyCrossPlatform::Encryption::Base64::base64Encode(EncodingAuthText);
		mRequestCls.m_ResquestContent.SetFieldWithSingleValue("Authorization", std::string("Basic ") + EncodedAuthText);
	}
	this->mResponseContent = mRequestCls.ResponseContent;
	mRequestCls.performRequest();
	if (!mRequestCls.SucceedRequest) {
		this->DownloadSucceed = false;
		return;
	}
	this->DownloadedData = EasyCrossPlatform::Parser::StringUtil::toBytes(mRequestCls.ResponseContent.OriginalData);
	switch (mRequestCls.ResponseContent.ResponseCode) {
	case 200U:
		this->DownloadSucceed = true;
		return;
		break;
	case 300U:
	case 301U:
	case 302U:
	case 307U:
	case 201U:
		auto LocationIt = mRequestCls.ResponseContent.FieldsValues.find("Location");
		if (LocationIt == mRequestCls.ResponseContent.FieldsValues.end()) {
			this->DownloadSucceed = false;
			return;
		}
		return this->PerformRequest(mRequestMethod, (*LocationIt).second[0], PostData, AuthData, RecursionTime + 1);
		break;
	case 303U:
		auto Location303It = mRequestCls.ResponseContent.FieldsValues.find("Location");
		if (Location303It == mRequestCls.ResponseContent.FieldsValues.end()) {
			this->DownloadSucceed = false;
			return;
		}
		return this->PerformRequest(RequestMethod::GET, (*Location303It).second[0], PostData, AuthData, RecursionTime + 1);
		break;
	case 204U:
	case 205U:
		this->DownloadSucceed = true;
		this->DownloadedData.clear();
		return;
	case 206U:
		//暂时不处理Partial Content
		this->DownloadSucceed = true;
		break;
	default: //400-511 Error Codes
		this->DownloadSucceed = false;
		break;
	}
	

}

void EasyCrossPlatform::Network::Request::WebPageDownload::cleanUp()
{
	DownloadedData.clear();
	mResponseContent.cleanUp();
	DownloadSucceed = false;
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformGetRequest(const std::string& URL)
{
	this->PerformRequest(RequestMethod::GET, URL, "", std::pair<std::string,std::string>(), 0U);
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformPostRequest(const std::string & URL, const std::string & PostData)
{
	this->PerformRequest(RequestMethod::POST, URL, PostData, std::pair<std::string,std::string>() ,0U);
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformPutRequest(const std::string & URL, const std::string & FileContent)
{
	this->PerformRequest(RequestMethod::PUT, URL, FileContent, std::pair<std::string, std::string>(), 0U);
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformDeleteRequest(const std::string & URL)
{
	this->PerformRequest(RequestMethod::DEL, URL, "", std::pair<std::string, std::string>(), 0U);
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformTraceRequest(const std::string & URL, const std::string & PostData)
{
	this->PerformRequest(RequestMethod::TRACE, URL, PostData, std::pair<std::string, std::string>(), 0U);
}
