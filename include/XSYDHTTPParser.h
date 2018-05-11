#ifndef __XSYDHTTPParserFile__
	#define __XSYDHTTPParserFile__
	#include <EasyCP_Common.h>
	#include <XSYDStringUtil.h>
	#include <XSYDCompression.h>
	#include <math.h>
	#include <cmath>

	namespace EasyCrossPlatform {
		namespace Parser {
			namespace HTTP {
				const constexpr char* HTTPNewLineStr = "\r\n";
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
				std::string EncodeChunckedData(const std::string& OriginalData);
				class URL {
					friend class HTTPRequestHeader;
					friend class HTTPResponseHeader;
					private:
						static unsigned char ToHex(unsigned char x);
						static unsigned char FromHex(unsigned char x);
					protected:
						
						static std::string UrlEncode(const std::string& szToEncode);
						static std::string UrlDecode(const std::string& szToDecode);
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
						std::map<std::string, std::vector<std::string>> FieldsValues;
						void SetFieldWithSingleValue(const std::string& FieldName, const std::string& FieldValue);
						void AppendFieldWithSingleValue(const std::string& FieldName, const std::string& AppendFieldValue);
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
						std::map<std::string, std::vector<std::string>> FieldsValues;
						std::vector<std::string> ContentEncoding;
						std::vector<std::string> TransferEncoding;
						std::vector<std::pair<std::string,float>> AcceptEncoding;
						void SetFieldWithSingleValue(const std::string& FieldName, const std::string& FieldValue);
						void AppendFieldWithSingleValue(const std::string& FieldName, const std::string& AppendFieldValue);
						std::string OriginalData = "";
						std::string EncodedData = "";
						std::string Connection = "";

						HTTPParseReturnVal fromReqString(const std::string& ReqString);
						std::string WriteHeader();
						std::string toReqString();
						void cleanUp();
						
				};
				class HTTPResponseHeader {
				private:

				protected:
					bool ParseFirstLine(const std::string& FirstLine);
					std::string WriteFirstLine();
					void AnalyzeField(const std::string& SingleLine);
					std::string WriteField(const std::string& FieldName);
				public:
					void cleanUp();
					unsigned int ResponseCode = 200U;
					std::string ResponseDescription = "OK";
					unsigned int MajorVersion = 1U;
					unsigned int MinorVersion = 1U;
					void SetFieldWithSingleValue(const std::string& FieldName, const std::string& FieldValue);
					void AppendFieldWithSingleValue(const std::string& FieldName, const std::string& AppendFieldValue);
					std::map<std::string, std::vector<std::string>> FieldsValues;
					HTTPParseReturnVal fromResponseString(const std::string& ResponseString); //Returns part of the string that is not parsed.
					std::string toResponseString();
				};
				
				class HTTPResponse {
				private:

				protected:
					HTTPParseReturnVal parseHeader(const std::string& ResponseString);
					void AnalyzeContentEncodingValue(const std::string& ContentEncodingVal);
					void AnalyzeTransferEncodingValue(const std::string& TransferEncodingVal);
					HTTPParseReturnVal DecodeData();

					std::string WriteContentEncodingValue();
					std::string WriteTransferEncodingValue();
					void EncodeData();
				public:
					unsigned int MajorVersion = 1U;
					unsigned int MinorVersion = 1U;
					unsigned int CompressionLevel = 9U;
					unsigned int ResponseCode = 200U;
					std::string ResponseDescription = "";
					std::map<std::string, std::vector<std::string>> FieldsValues;
					std::vector<std::string> ContentEncoding;
					std::vector<std::string> TransferEncoding;
					std::string OriginalData = "";
					std::string EncodedData = "";
					std::string Connection = "";
					void SetFieldWithSingleValue(const std::string& FieldName, const std::string& FieldValue);
					void AppendFieldWithSingleValue(const std::string& FieldName, const std::string& AppendFieldValue);
					HTTPParseReturnVal fromResponseString(const std::string& ResponseString);
					std::string WriteHeader();
					std::string toResponseString();
					void cleanUp();
				};
			}
		}
	}
#endif