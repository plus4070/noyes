#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include "TNS.h"
#include "TopicNameSpaceTable.h"
#include <process.h>
#include <deque>

using namespace std;

namespace CNU_DDS
{

	void	initalizeTNSServer();

	SOCKET	CreateSocket();
	void	BindingSocket(SOCKET servSocket) ;
	void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	void	StartReceving();
	void	startServer();
	void	endServer();
	
	static UINT WINAPI receiving(LPVOID p);
	static UINT WINAPI storing(LPVOID p);

	void inputDummy(TopicNameSpaceTable * TNSPTable);
	void initalFrontEndServer();
}

#endif
