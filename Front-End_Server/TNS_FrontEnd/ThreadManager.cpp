#include "StdAfx.h"
#include "ThreadManager.h"


ThreadManager::ThreadManager(void)
{
}


ThreadManager::~ThreadManager(void)
{
}


HANDLE		ThreadManager::GetThread(int ThreadID){
	switch(ThreadID){
	case THREAD_PUBLICATION_MONITORING:
		return this->PublicationMonitoringThread;
		break;
	case THREAD_SUBSCRIPTION_MONITORING:
		return this->SubscriptionMonitoringThread;
		break;
	case THREAD_FRONTEND_SERVICE:
		return this->FrontEndServiceThread;
		break;
	case THREAD_RECEVING:
		return this->RecevingThread;
		break;
	case THREAD_APPLICATION:
		return this->ApplicationTableThread;
		break;
	}
}

TNSTable*	ThreadManager::GetTNSTable(){
	return this->Table;
}

void		ThreadManager::SetThread(HANDLE thread, int ThreadID){
	switch(ThreadID){
	case THREAD_PUBLICATION_MONITORING:
		this->PublicationMonitoringThread = thread;
		break;
	case THREAD_SUBSCRIPTION_MONITORING:
		this->SubscriptionMonitoringThread = thread;
		break;
	case THREAD_FRONTEND_SERVICE:
		this->FrontEndServiceThread = thread;
		break;
	case THREAD_RECEVING:
		this->RecevingThread = thread;
		break;
	case THREAD_APPLICATION:
		this->ApplicationTableThread = thread;
		break;
	}
}

void		ThreadManager::SetTNSTable(TNSTable * table){
	this->Table = table;
}

void		ThreadManager::Suspend(int ThreadID){
	HANDLE suspendHandle;
	switch(ThreadID){
	case THREAD_PUBLICATION_MONITORING:
		suspendHandle = this->PublicationMonitoringThread;
		break;
	case THREAD_SUBSCRIPTION_MONITORING:
		suspendHandle = this->SubscriptionMonitoringThread;
		break;
	
	case THREAD_FRONTEND_SERVICE:
		suspendHandle = this->FrontEndServiceThread;
		break;
	
	case THREAD_RECEVING:
		suspendHandle = this->RecevingThread;
		break;

	case THREAD_APPLICATION:
		suspendHandle = this->ApplicationTableThread;
		break;
	}

	SuspendThread(suspendHandle);
}

void		ThreadManager::Resume(int ThreadID){
	HANDLE ResumeHandle;
	int ResumeCount;
	
	switch(ThreadID){
	case THREAD_PUBLICATION_MONITORING:
		ResumeHandle = this->PublicationMonitoringThread;
		break;
	case THREAD_SUBSCRIPTION_MONITORING:
		ResumeHandle = this->SubscriptionMonitoringThread;
		break;
	
	case THREAD_FRONTEND_SERVICE:
		ResumeHandle = this->FrontEndServiceThread;
		break;
	
	case THREAD_RECEVING:
		ResumeHandle = this->RecevingThread;
		break;

	case THREAD_APPLICATION:
		ResumeHandle = this->ApplicationTableThread;
		break;
	}

	do{
		ResumeCount = ResumeThread(ResumeHandle); 
	}while(ResumeCount > 0);
}