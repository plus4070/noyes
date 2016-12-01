#pragma once
#include <WinSock2.h>
#include <process.h>
#include "StdAfx.h"
#include "TNSTable.h"

class ThreadManager
{
public:
	ThreadManager(void);
	~ThreadManager(void);

	HANDLE		GetThread(int ThreadID);
	TNSTable*	GetTNSTable();

	void		SetThread(HANDLE thread, int ThreadID);
	void		SetTNSTable(TNSTable * table);

	void		Suspend(int ThreadID);
	void		Resume(int ThreadID);
	
private:
	HANDLE		PublicationMonitoringThread;
	HANDLE		SubscriptionMonitoringThread;
	HANDLE		RecevingThread;
	HANDLE		FrontEndServiceThread;
	HANDLE		ApplicationTableThread;
	
	TNSTable	*Table;
};

