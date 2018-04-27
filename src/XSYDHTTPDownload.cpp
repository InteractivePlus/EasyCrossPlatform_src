#include <XSYDHTTPDownload.h>

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformRequest(const RequestMethod mRequestMethod, const std::string& URL, const std::string& PostData, const unsigned int RecursionTime)
{
	WebsiteRequest mRequestCls = WebsiteRequest();
	if (RecursionTime >= Request::MaxRedirectTime) {
		this->DownloadSucceed = false;
		return;
	}
	mRequestCls.cleanRequest();
	mRequestCls.cleanUp();
	this->cleanUp();
	switch (mRequestMethod) {
	case RequestMethod::GET:
		mRequestCls.Method = RequestMethod::GET;
		break;
	case RequestMethod::POST:
		mRequestCls.Method = RequestMethod::POST;
		mRequestCls.RequestData = PostData;
		break;
	default:
		this->DownloadSucceed = false;
		return;
		break;
	}
	mRequestCls.RequestURL = URL;
	mRequestCls.m_ResquestContent.FieldsValues["Accept-Language"] = this->AcceptLanguageString;
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
		if (mRequestCls.ResponseContent.FieldsValues.find("Location") == mRequestCls.ResponseContent.FieldsValues.end()) {
			this->DownloadSucceed = false;
			return;
		}
		return this->PerformRequest(mRequestMethod, mRequestCls.ResponseContent.FieldsValues["Location"], PostData, RecursionTime + 1);
		break;
	case 303U:
		if (mRequestCls.ResponseContent.FieldsValues.find("Location") == mRequestCls.ResponseContent.FieldsValues.end()) {
			this->DownloadSucceed = false;
			return;
		}
		return this->PerformRequest(RequestMethod::GET, mRequestCls.ResponseContent.FieldsValues["Location"], PostData, RecursionTime + 1);
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
	this->PerformRequest(RequestMethod::GET, URL, "", 0U);
}

void EasyCrossPlatform::Network::Request::WebPageDownload::PerformPostRequest(const std::string & URL, const std::string & PostData)
{
	this->PerformRequest(RequestMethod::POST, URL, PostData, 0U);
}
