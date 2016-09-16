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

	int							numOfRequests;
	PR_NODE				r_head;
	vector<_R_ENTRY>	requestList;

	void							resetTable();
	bool							isRequestExist();														//Request 존재여부 확인
	PR_NODE				getLastEntry();														//마지막 Entry 리턴
	void							addEntry(IN_ADDR ip, PDD_NODE PDD_NODE);	//리스트에 Entry 추가

private:
	CRITICAL_SECTION cs;																			//Semaphore 역할
};

