#include "stdafx.h"
#include "ViewManager.h"
#include "SocketManager.h"
#include "DBManager.h"
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


private:
	SocketManager					* socketManager;
	DBManager						* databaseManager;

	MessageHandler					* messageHandler;

	CRITICAL_SECTION				cs;
	deque<pair<IN_ADDR, PDD_NODE>>	recvData;

	void 			initalizeSetting();
	void			startTNSServer();
	void			closeTNSServer();
	void			distibuteTNSData();
	bool			isReceviedDataExist();
};

