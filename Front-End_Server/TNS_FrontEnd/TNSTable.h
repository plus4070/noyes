#pragma once
#include "StdAfx.h"
#include <sstream>

using namespace std;

#define delim '/'

class TNSTable
{
public:
	int								CountOfID;
	vector<_TNSP_ENTRY>				requestList;
	vector<_TNSP_ENTRY>::iterator	it;

	TNSTable();
	~TNSTable();

	void			ResetTable();

	void			AddEntry(char topic[MAX_CHAR], char topictype[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port, unsigned char data[MAX_DATA_SIZE], unsigned char orderingType, int length,int nodetype, int messagetype);
	void			RemoveEntry(int id);
	void			SetEntry(int id, char addr[ADDRESS_SIZE], int state);
	TNSP_ENTRY		MakeTopicNameEntry(char topic[MAX_CHAR], char topictype[MAX_CHAR], char domain[MAX_CHAR], char addr[ADDRESS_SIZE], char paddr[ADDRESS_SIZE], int port, unsigned char data[MAX_DATA_SIZE], unsigned char orderingType, int length, int nodetype, int messagetype);

	bool			IsEntryExist(int state);
	PTNSP_ENTRY		GetEntry(int state);

	void			TestShowAll();

	vector<string>	SplitTopic(const string & s);
	int				GetNumOfRequests();
};
