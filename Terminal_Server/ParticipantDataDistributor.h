#include "stdafx.h"
#include "TerminalTable.h"
#include <vector>
#include <algorithm>
#include <iostream>


#pragma once

class ParticipantDataDistributor
{
public:
	ParticipantDataDistributor();
	~ParticipantDataDistributor();
	void							setPubSubList(TerminalTable * List);
	bool							checkModifyTableEntry();
	list<pair<IN_ADDR, PDD_DATA>>	getParticipantData();

private:
	TerminalTable * ParticipantList;
};

