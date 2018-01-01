#ifndef __XSYDJSONFILE__
	#define __XSYDJSONFILE__
	#include "EasyCP_Common.h"
	#include "json/json.h"
	namespace EasyCrossPlatform {
		namespace Parser {
			namespace JSON {
				Json::Value ReadFromJSON(const std::string& JSONString);
				std::string WriteToJSON(const Json::Value& JSONObj);
			}
		}
	}
#endif