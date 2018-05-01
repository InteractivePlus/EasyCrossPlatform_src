#include <XSYDRegex.h>

std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo> EasyCrossPlatform::Parser::Regex::MatchRegex(const std::string & TextToMatch, const std::string & RegexExpr)
{
	std::regex express = std::regex(RegexExpr);
	std::match_results<std::string::const_iterator> results;
	std::vector<RegexInfo> mResult = std::vector<RegexInfo>();
	bool MatchSucceed = std::regex_match(TextToMatch.cbegin(), TextToMatch.cend(), results, express);
	if(MatchSucceed){
		std::match_results<std::string::const_iterator>::const_iterator iter;
		mResult.reserve(results.size());
		for (iter = results.begin(); iter != results.end(); iter++) {
			RegexInfo mInfo;
			mInfo.MatchedText = iter->str();
			mInfo.StartIndex = std::distance(TextToMatch.cbegin(), iter->first);
			mResult.push_back(mInfo);
		}
	}
	return mResult;
}

std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo> EasyCrossPlatform::Parser::Regex::SingleSearchRegex(const std::string & TextToMatch, const std::string & RegexExpr)
{
	std::regex express = std::regex(RegexExpr);
	std::match_results<std::string::const_iterator> results;
	std::vector<RegexInfo> mResult = std::vector<RegexInfo>();
	bool SearchSucceed = std::regex_search(TextToMatch.cbegin(), TextToMatch.cend(), results, express);
	if (SearchSucceed) {
		std::match_results<std::string::const_iterator>::const_iterator iter;
		for (iter = results.begin(); iter != results.end(); iter++) {
			RegexInfo mInfo;
			mResult.reserve(results.size());
			mInfo.MatchedText = iter->str();
			mInfo.StartIndex = std::distance(TextToMatch.cbegin(), iter->first);
			mResult.push_back(mInfo);
		}
	}
	return mResult;
}

std::vector<std::vector<EasyCrossPlatform::Parser::Regex::RegexInfo>> EasyCrossPlatform::Parser::Regex::MultipleSearchRegex(const std::string & TextToMatch, const std::string & RegexExpr, const int Limit)
{
	std::string::size_type mStartPos = 0U;
	std::regex express = std::regex(RegexExpr);
	std::match_results<std::string::const_iterator> results;
	std::vector<std::vector<RegexInfo>> mResult = std::vector<std::vector<RegexInfo>>();
	while (mStartPos != std::string::npos) {
		if (mResult.size() >= Limit && Limit != -1) {
			break;
		}
		bool SearchSucceed = std::regex_search(TextToMatch.cbegin()+mStartPos, TextToMatch.cend(), results, express);
		if (SearchSucceed) {
			std::match_results<std::string::const_iterator>::const_iterator iter;
			std::vector<RegexInfo> mTempRst = std::vector<RegexInfo>();
			mTempRst.reserve(results.size());
			for (iter = results.begin(); iter != results.end(); iter++) {
				RegexInfo mInfo;
				mInfo.MatchedText = iter->str();
				mInfo.StartIndex = std::distance(TextToMatch.cbegin(), iter->first);
				mTempRst.push_back(mInfo);
				mStartPos = mInfo.StartIndex + iter->length();
				if (mStartPos > TextToMatch.length() - 1) {
					mStartPos = std::string::npos;
				}
			}
			mResult.push_back(mTempRst);
		}
		else {
			mStartPos = std::string::npos;
		}
	}
	return mResult;
}

std::string EasyCrossPlatform::Parser::Regex::ReplaceRegexWithString(const std::string & Text, const std::string & RegexExpr, const std::string & ReplaceStr, const int Limit)
{
	std::string ResultStr = Text;
	int IndexShift = 0;
	std::vector<std::vector<RegexInfo>> mSearchResult = Regex::MultipleSearchRegex(Text, RegexExpr, Limit);
	for (std::vector<RegexInfo> mRegexArr : mSearchResult) {
		RegexInfo mRegexInfo = mRegexArr[0];
		ResultStr.replace(mRegexInfo.StartIndex + IndexShift, mRegexInfo.MatchedText.length(), ReplaceStr);
		IndexShift += ReplaceStr.length() - mRegexInfo.MatchedText.length();
	}
	return ResultStr;
}
