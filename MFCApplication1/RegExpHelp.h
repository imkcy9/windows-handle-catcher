#pragma once

#include <regex>
#include <string>

class RegExpHelp
{
public:
	enum EXP_TYPE {
		GET_ID = 0,
		GET_LAST_PRICE,
		GET_BID_PRICE,
		GET_ASK_PRICE,
		GET_TIME
	};
	RegExpHelp();
	~RegExpHelp();

	std::string getMatch(std::string& input, EXP_TYPE type);

	std::string getSearchIndexResult(std::string& input, std::string& pattern, int index);
private:
	std::string getSecurityID(std::string& input);
	std::string getLastPrice(std::string& input);
	std::string getBidPrice(std::string& input);
	std::string getAskPrice(std::string& input);
	std::string getTime(std::string& input);
};

