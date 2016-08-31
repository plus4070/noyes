#pragma once

#include "stdafx.h"

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#include <vector>
#include <sstream>
#include <string>

#define delim '/'

using namespace std;


class TopicNameTable
{
public:
	TopicNameTable();

	virtual		~TopicNameTable();
	void		resetTable();


	bool			isEntryExist(TN_ENTRY entry);
	bool			addEntry(TN_ENTRY entry);
	bool			deleteEntry(TN_ENTRY entry);
	bool			getEntry(TN_ENTRY *p_entry);
	bool			setEntry(TN_ENTRY *p_entry);

	void			testShowAll();
	vector<string>	splitTopic(const string & s);

	PTN_NODE TN_head;
};

