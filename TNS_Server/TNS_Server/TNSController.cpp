#include "TNSController.h"

TNSController::TNSController()
{
	initalizeSetting();
}


TNSController::~TNSController()
{
	closeTNSServer();
	DeleteCriticalSection(&cs);
}

void TNSController::initalizeSetting() 
{
	this->socketManager = new SocketManager();
	this->messageHandler = new MessageHandler();

	InitializeCriticalSection(&cs);
	this->socketManager->setCriticalSection(&cs);
}

void TNSController::startTNSServer() 
{
	this->socketManager->getRecevingDEQUE(&(this->recvData));
	this->socketManager->startRecevingThread();
}

void TNSController::closeTNSServer() 
{
	this->socketManager->closeRecevingThread();
}

bool TNSController::isReceviedDataExist() 
{
	return this->recvData->size() == 0 ? false : true;
}