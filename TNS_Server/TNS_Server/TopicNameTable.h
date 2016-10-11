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
	virtual	~TopicNameTable();
	
	void	ResetTable();
	bool	IsEntryExist(TN_ENTRY entry);
	bool	AddEntry(TN_ENTRY entry);
	bool	GetEntry(TN_ENTRY *p_entry);

	void			TestShowAll();
	vector<string>	SplitTopic(const string & s);

	PTN_NODE TN_head;
};

