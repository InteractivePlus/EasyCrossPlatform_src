#ifndef __XSYDHTTPParserFile__
	#define __XSYDHTTPParserFile__
	#include <EasyCP_Common.h>
	#include <httpParser/http_parser.h>
	#include <XSYDStringUtil.h>
	#include <XSYDCompression.h>

	namespace EasyCrossPlatform {
		namespace Parser {
			namespace HTTP {
				const constexpr char* AcceptEncoding[2] = { "gzip","br" };
				const constexpr unsigned int MaxReqAnalyzeSize = 256U;
				const constexpr unsigned int ChunckedSplitSize = 512U;
				struct HTTPParseReturnVal {
					bool msgIsHTTP = false;
					bool msgIsEnough = true;
					bool canDecode = true;
					bool onError = false;
					std::string RemainMsg = "";
				};
				unsigned int FromHexStringToDec(const std::string& HexString);
				std::string FromDecToHexString(const unsigned int Num);
				HTTPParseReturnVal DecodeChunckedData(const std::string& EncryptedData, std::string& DataForWriting);
				class URL {
					private:

					protected:
						
					public:
						std::string Protocol = ""; //Example: http
						std::string AccessCert = ""; //Example: Empty / admin:xsyd666
						std::string Host = ""; //Example: www.xsyds.cn
						unsigned short Port = 0U; //Example: 80U
						std::string Path = ""; //Example: "abc/def"
						std::string QueryString = ""; //Example: abc=1
						std::string FileName = ""; //Example=index.html
						std::string FragmentID = ""; //Example=div1
						std::string toURLString();
						void fromURLString(const std::string& urlString);
						void setAccessCert(const std::string& Username, const std::string& Password);
						std::pair<std::string, std::string> getAccessCert();
						void cleanUp();
				};
				class HTTPRequestHeader {
					private:

					protected:
						bool ParseFirstLine(const std::string& FirstLine);
						std::string WriteFirstLine();
						void AnalyzeField(const std::string& SingleLine);
						std::string WriteField(const std::string& FieldName);
					public:
						void cleanUp();
						unsigned int MajorVersion = 1U;
						unsigned int MinorVersion = 1U;
						std::string Method = "GET";
						URL RequestedURL;
						std::map<std::string, std::string> FieldsValues;
						HTTPParseReturnVal fromReqString(const std::string& ReqString); //Returns part of the string that is not parsed.
						std::string toReqString();
				};
				class HTTPRequest {
					private:

					protected:
						HTTPParseReturnVal parseHeader(const std::string& ReqString);
						void AnalyzeAcceptEncodingValue(const std::string& AcceptEncodingVal);
						void AnalyzeContentEncodingValue(const std::string& ContentEncodingVal);
						void AnalyzeTransferEncodingValue(const std::string& TransferEncodingVal);
						HTTPParseReturnVal DecodeData();
						
						std::string WriteAcceptEncodingValue();
						std::string WriteContentEncodingValue();
						std::string WriteTransferEncodingValue();
						void EncodeData();
					public:
						unsigned int MajorVersion = 1U;
						unsigned int MinorVersion = 1U;
						unsigned int CompressionLevel = 9U;
						std::string Method = "GET";
						URL RequestedURL;
						std::map<std::string, std::string> FieldsValues;
						std::vector<std::string> ContentEncoding;
						std::vector<std::string> TransferEncoding;
						std::vector<std::pair<std::string,float>> AcceptEncoding;
						std::string OriginalData = "";
						std::string EncodedData = "";
						std::string Connection = "";

						HTTPParseReturnVal fromReqString(const std::string& ReqString);
						std::string WriteHeader();
						std::string toReqString();
						void cleanUp();
						
				};
			}
		}
	}
#endif