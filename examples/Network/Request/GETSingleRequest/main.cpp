#include <EasyCrossPlatform.h>
#include <iostream>

int main(int argc, char** args) {
	/*
	Making an HTTP request causes the current thread to block, if you are doing a proxy server or etc, we recommend you to use it in a different thread and think-async!
	*/
	//Note that Request class only do ONE request, if you want precise result after the page is directed from 301, 302 status code, you should use HTTPDownload
	EasyCrossPlatform::Network::Request::WebsiteRequest mRequest;
	mRequest.Method = EasyCrossPlatform::Network::Request::RequestMethod::GET;
	//mRequest.RequestURL = "https://www.httpwatch.com/httpgallery/chunked/chunkedimage.aspx";
	mRequest.RequestURL = "https://www.xsyds.cn/en/";
	mRequest.m_ResquestContent.FieldsValues["Accept-Language"] = "zh-CN;q=1.0, en-US;q=0.8, *;q=0";
	system("pause");
	double StartTime = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
	std::cout << "Requesting..." << std::endl;
	mRequest.performRequest();
	double StopTime = EasyCrossPlatform::Runtime::Chrono::accurateTimer();
	std::cout << std::to_string(StopTime - StartTime) << " seconds" << std::endl;
	std::cout << "Request " << (mRequest.SucceedRequest ? "succeeded" : "failed") << std::endl;
	//std::cout << "--------------------" << std::endl;
	//std::cout << "Response:" << std::endl;
	//std::cout << mRequest.ResponseOriginalData << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "Response Body:" << std::endl;
	std::cout << mRequest.ResponseContent.OriginalData << std::endl;
	std::cout << "--------------------" << std::endl;
	std::cout << "Remaining Msg:" << std::endl;

	system("pause");
}