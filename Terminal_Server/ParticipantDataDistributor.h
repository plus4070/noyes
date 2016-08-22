#include "stdafx.h"
#include "TerminalTable.h"
#include <vector>

#pragma once

class ParticipantDataDistributor
{
public:
	ParticipantDataDistributor();
	~ParticipantDataDistributor();
	void setPubSubList(TerminalTable * List);
	bool checkModifyTableEntry();
	vector<IN_ADDR> getParticipantData(PPDD_NODE Packet, int type);

private:
	TerminalTable * ParticipantList;
};

