#include "stdafx.h"
#include"ViewManager.h"
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

	void	startTNSServer();

private:
	SocketManager	* socketManager;
	ViewManager	* viewManager;
	MessageHandler	* messageHandler;

	CRITICAL_SECTION	cs;
	deque<pair<IN_ADDR, PDD_NODE>>	* recvData;

	void	initalizeSetting();
	void	closeTNSServer();
	bool	isReceviedDataExist();
};
