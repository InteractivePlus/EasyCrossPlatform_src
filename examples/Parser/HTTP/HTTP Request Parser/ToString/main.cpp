#include <EasyCrossPlatform.h>
#include <iostream>
using namespace EasyCrossPlatform::Parser;
int main(int argc, char** args) {
	HTTP::HTTPRequest mRequestCls;
	mRequestCls.RequestedURL.fromURLString("https://www.google.com/search?safe=strict&client=firefox-b-ab&ei=N--yWpnWDpKo_QaluYKYBg&q=oxymoron+example&oq=oxymoron+example&gs_l=psy-ab.3..0l4j0i30k1l6.880.2602.0.2910.10.6.0.4.4.0.104.340.3j1.4.0....0...1c.1.64.psy-ab..2.7.284...0i10i19k1j0i12i10i30i19k1.0.Z1c-UcKhyEo");
	mRequestCls.AcceptEncoding.push_back(std::pair<std::string, float>("br", 1.0F));
	mRequestCls.AcceptEncoding.push_back(std::pair<std::string, float>("gzip", 0.9F));
	mRequestCls.AcceptEncoding.push_back(std::pair<std::string, float>("deflate", 0.8F));
	mRequestCls.AcceptEncoding.push_back(std::pair<std::string, float>("identity", 0.1F));
	mRequestCls.ContentEncoding.push_back("br");
	mRequestCls.OriginalData = "2333CIWOJFOIJOIMOCMKNWIOFINOIZMIOWIOFNI";
	std::cout << "ToReqString:" << std::endl;
	std::cout << mRequestCls.toReqString() << std::endl;
	std::cout << "----------------" << std::endl;
	mRequestCls.cleanUp();
	system("pause");
}