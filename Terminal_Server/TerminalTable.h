#include "stdafx.h"
#pragma once

#define MAX_CHAR 100
#define EDP_DATA_SIZE 100
#define PDP_DATA_SIZE 100
#define NULL 0

using namespace std;


class TerminalTable {

public:

	TerminalTable();

	void	resetTable();
	bool	isEntryExist(T_ENTRY entry);
	bool	addTopic(T_ENTRY entry);
	bool	deleteTopic(T_ENTRY entry);

	PT_NODE t_head;


	void	test_showAllEntry();
};
