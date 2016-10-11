#include "stdafx.h"
#include "TopicNameTable.h"
#include"SocketManager.h"
#include "MessageHandler.h"
#include <process.h>
#include <deque>
#include <list>

#pragma once

class TNSController
{
public:
	TNSController();
	~TNSController();

	void	StartTNSServer();

private:
	TopicNameTable	*TNTable;
	SocketManager		*socketManager;
	MessageHandler	*messageHandler;

	CRITICAL_SECTION	cs;
	deque<pair<IN_ADDR, PDD_NODE>>	*recvData;

	void	InitializeView();
	void	InitializeSetting();
	void	InputDummyEntryToTNTable();
	void	CloseTNSServer();
	bool	IsReceviedDataExist();
};
