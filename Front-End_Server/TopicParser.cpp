#include "TopicParser.h"

vector<string> TopicParser::split(const string & s)
{
	stringstream ss(s);
	string item;
	vector<string> tokens;
 	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}