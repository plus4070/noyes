
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#pragma once

using namespace std;

class TopicParser{
public:
	char delim = '/';

	vector<string>	split(const string &s);
};
 