#include <EasyCrossPlatform.h>
#include <iostream>

int main(int argc, char** args) {
	EasyCrossPlatform::Network::Request::WebsiteRequest mRequest;
	mRequest.Method = EasyCrossPlatform::Network::Request::RequestMethod::GET;
	//mRequest.RequestURL = "https://www.httpwatch.com/httpgallery/chunked/chunkedimage.aspx";
	mRequest.RequestURL = "https://www.xsyds.cn/en/";
	mRequest.m_ResquestContent.FieldsValues["Accept-Language"] = "zh-CN;q=1.0, en-US;q=0.8, *;q=0";
	system("pause");
	std::cout << "Requesting..." << std::endl;
	mRequest.performRequest();
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