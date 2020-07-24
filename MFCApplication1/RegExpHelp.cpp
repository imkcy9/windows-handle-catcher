#include "stdafx.h"
#include "RegExpHelp.h"
#include <regex>


RegExpHelp::RegExpHelp()
{
	std::cmatch m;
	std::regex_match("<xml>value</xml>", m, std::regex("<(.*)>(.*)</(\\1)>"));
}


RegExpHelp::~RegExpHelp()
{
}

std::string RegExpHelp::getMatch(std::string & input, EXP_TYPE type)
{
	
	std::string result;
	switch (type) {
	case GET_ID:
		result = this->getSecurityID(input);
		
		break;
	case GET_LAST_PRICE:
		break;
	case GET_BID_PRICE:
		break;
	case GET_ASK_PRICE:
		break;
	case GET_TIME:
		break;
	default:
		break;
	}
	return result;
}

std::string RegExpHelp::getSearchIndexResult(std::string & input, std::string & pattern, int index)
{
	std::cmatch m;
	//std::regex_match(input.c_str(), m, std::regex("([a-zA-Z]*):"));
	// CRSP: 92.22 (3h33m/1m)  B:92.22 A:92.29

	try {
		std::regex_search(input.c_str(), m, std::regex(pattern));
	}
	catch (std::regex_error& e) {
		printf("regex_error %s", e.what());
		throw e;
	}
	

	if (m.size() == 0) {
		return input;
	}

	if (index <= 0) {
		return m[0];
	}

	if ((size_t)index >= m.size()) {
		return m[m.size() - 1];
	}

	return m[index];
}

std::string RegExpHelp::getSecurityID(std::string & input)
{
	std::cmatch m;
	//std::regex_match(input.c_str(), m, std::regex("([a-zA-Z]*):"));
	// CRSP: 92.22 (3h33m/1m)  B:92.22 A:92.29
	std::regex_search("CRSP: 92.22 (3h33m/1m)  B:92.22 A:92.29", m, std::regex("^(.*): ?(.*) *B:(.*) *A:(.*)"));
	std::string result = m.size() > 0 ? m[1].str() : "";
	if (!result.empty() && result.back() == ':') {
		result.pop_back();
	}
	return result;
}

std::string RegExpHelp::getLastPrice(std::string & input)
{
	return std::string();
}

std::string RegExpHelp::getBidPrice(std::string & input)
{
	return std::string();
}

std::string RegExpHelp::getAskPrice(std::string & input)
{
	return std::string();
}

std::string RegExpHelp::getTime(std::string & input)
{
	return std::string();
}
