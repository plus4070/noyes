#pragma once

#include <stdlib.h>
#include <vector>
#include "stdafx.h"

using namespace std;


class RequestTable
{
public:
	RequestTable();
	~RequestTable();
	void				resetTable();

	PR_NODE		r_head;

	vector<_R_ENTRY>	requestList;
	int					numOfRequests;

	bool				isRequestExist();
	PR_NODE				getLastEntry();
	void				addEntry(IN_ADDR ip, TNSN_ENTRY message);


private:
	CRITICAL_SECTION cs;
};

