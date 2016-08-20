#include "stdafx.h"
#include <vector>
#pragma once

#define MAX_CHAR 100
#define EDP_DATA_SIZE 100
#define PDP_DATA_SIZE 100
#define NULL 0

using namespace std;


class TerminalTable {

public:

	TerminalTable();

	void				resetTable();
	bool				isEntryExist(T_ENTRY entry);
	bool				addTopic(T_ENTRY entry);
	bool				deleteTopic(T_ENTRY entry);
		
	void				resetModifyFlag();
	bool				isTableModified();

	int					getAllModifiedData(PDD_DATA dataArray[MAX_PDD_NUMBER]);
	void				test_showAllEntry();

	vector<IN_ADDR>		getAllAddressList();

private:
	PT_NODE t_head;
	bool	isModifyEntryExist;
};
