#include "StdAfx.h"
#include "ThreadManager.h"

#pragma once
class DDSMonitoring
{
public:
	DDSMonitoring(ThreadManager * TM);
	~DDSMonitoring(void);

	void startMonitoring();
	void closeMonitoring();

private:
	HANDLE pubMonitor;
	HANDLE subMonitor;
	ThreadManager * tm;
};

