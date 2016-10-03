#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include "TNSNheader.h"
#include "TNSTable.h"
#include <process.h>
#include <deque>

using namespace std;

namespace CNU_DDS
{
	void	initalizeTNSServer();
	void	initalDomainForTNS();
	void	start();
	SOCKET	CreateSocket();
	void	BindingSocket(SOCKET servSocket);
	void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	void	StartReceving();
	void	startServer();
	void	endServer();

	void initalFrontEndServer();
}

#endif
