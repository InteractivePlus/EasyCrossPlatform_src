#include "XSYDRequest.h"
using namespace std;

size_t EasyCrossPlatform::Network::Request::WebsiteRequest::writeFunction(void * ptr, size_t dataTypesize, size_t dataNum, void * stream)
{
	//这里传来的肯定是string的ptr
	size_t realSize = dataTypesize * dataNum;
	//先转为Char, 再进行处理
	char* myCharData = new char[realSize];
	std::memcpy(myCharData, ptr, realSize);
	
	std::string *myString = (std::string*) stream;
	myString->clear();
	myString->assign(myCharData, realSize);
	delete[] myCharData;
	return realSize;
}

std::string EasyCrossPlatform::Network::Request::WebsiteRequest::convertPostMapToString(const std::map<std::string, std::string> &myMap)
{
	if (myMap.empty()) {
		return std::string();
	}
	std::string myResult;
	for (auto i : myMap) {
		myResult += i.first + "=" + i.second + "&";
	}
	myResult.pop_back();//删除最后一个&
	return myResult;
}

EasyCrossPlatform::Network::Request::WebsiteRequest::WebsiteRequest()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

EasyCrossPlatform::Network::Request::WebsiteRequest::~WebsiteRequest()
{
	curl_global_cleanup();
}

EasyCrossPlatform::Network::Request::WebsiteRequestInfo EasyCrossPlatform::Network::Request::WebsiteRequest::getURL(const std::string &URL, bool SSLCertCheck)
{
	EasyCrossPlatform::Network::Request::WebsiteRequestInfo myResult;
	this->m_myThreadMutex.lock();
	this->m_myCurl = curl_easy_init();
	if (this->m_myCurl == NULL) {
		myResult.Succeed = false;
		return myResult;
	}
	curl_easy_setopt(this->m_myCurl, CURLOPT_URL, URL.c_str());
	if (!SSLCertCheck) {
		curl_easy_setopt(this->m_myCurl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(this->m_myCurl, CURLOPT_SSL_VERIFYHOST, 0);
	}
	curl_easy_setopt(this->m_myCurl, CURLOPT_TIMEOUT, 30); //30 seconds for request
	curl_easy_setopt(this->m_myCurl, CURLOPT_WRITEFUNCTION, this->writeFunction);
	std::string myHeaderData, myBodyData;
	curl_easy_setopt(this->m_myCurl, CURLOPT_HEADERDATA, &myHeaderData);
	curl_easy_setopt(this->m_myCurl, CURLOPT_WRITEDATA, &myBodyData);
	CURLcode myCurlCode = curl_easy_perform(this->m_myCurl);
	myResult.Body = myBodyData;
	myResult.Header = myHeaderData;
	myResult.ErrNo = myCurlCode;
	if (myResult.ErrNo == CURLE_OK) {
		myResult.Succeed = true;
		curl_easy_getinfo(this->m_myCurl, CURLINFO_RESPONSE_CODE, &(myResult.ServerResponseCode));
	}
	curl_easy_cleanup(this->m_myCurl);
	this->m_myCurl = NULL;
	this->m_myThreadMutex.unlock();
	return myResult;
}

EasyCrossPlatform::Network::Request::WebsiteRequestInfo EasyCrossPlatform::Network::Request::WebsiteRequest::postURL(const std::string & URL, const std::map<std::string, std::string>& PostValue, bool SSLCertCheck)
{
	EasyCrossPlatform::Network::Request::WebsiteRequestInfo myResult;
	this->m_myThreadMutex.lock();
	this->m_myCurl = curl_easy_init();
	if (this->m_myCurl == NULL) {
		myResult.Succeed = false;
		return myResult;
	}
	curl_easy_setopt(this->m_myCurl, CURLOPT_URL, URL.c_str());
	if (!SSLCertCheck) {
		curl_easy_setopt(this->m_myCurl, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(this->m_myCurl, CURLOPT_SSL_VERIFYHOST, 0);
	}
	curl_easy_setopt(this->m_myCurl, CURLOPT_TIMEOUT, 30); //30 seconds for request
	curl_easy_setopt(this->m_myCurl, CURLOPT_POSTFIELDS,this->convertPostMapToString(PostValue).c_str());
	curl_easy_setopt(this->m_myCurl, CURLOPT_WRITEFUNCTION, this->writeFunction);
	std::string myHeaderData, myBodyData;
	curl_easy_setopt(this->m_myCurl, CURLOPT_HEADERDATA, &myHeaderData);
	curl_easy_setopt(this->m_myCurl, CURLOPT_WRITEDATA, &myBodyData);
	CURLcode myCurlCode = curl_easy_perform(this->m_myCurl);
	myResult.Body = myBodyData;
	myResult.Header = myHeaderData;
	myResult.ErrNo = myCurlCode;
	if (myResult.ErrNo == CURLE_OK) {
		myResult.Succeed = true;
		curl_easy_getinfo(this->m_myCurl, CURLINFO_RESPONSE_CODE, &(myResult.ServerResponseCode));
	}
	curl_easy_cleanup(this->m_myCurl);
	this->m_myCurl = NULL;
	this->m_myThreadMutex.unlock();
	return myResult;
}
