#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include "TNSNheader.h"
#include "TNSTable.h"
#include <process.h>
#include <deque>

#define FIRST_INDEX				0

#define SLEEP_TIME_SHORT				100

using namespace std;

namespace CNU_DDS
{
	void	InitalizeTNSServer();
	
	SOCKET	CreateSocket();
	void	BindingSocket(SOCKET servSocket);
	void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	void	StartServer();
	void	EndServer();
	/*void	StartReceiving();*/

	static UINT WINAPI Receiving(LPVOID p);
	static UINT WINAPI Storing(LPVOID p);

	void OpenConnection(vector<SOCKET> hSockArray, vector<WSAEVENT>	hEventArray, int index, int sockTotal);
	void TransferData(LPVOID p, SOCKET * sockArray, int index);
	void CloseConnection(vector<SOCKET> hSockArray, vector<WSAEVENT>	hEventArray, SOCKET * sockArray, WSAEVENT * eventArray, int index);

	void SeperateMessagePerType(deque<PDD_NODE> * recvQueue, TNSTable * TNST, PDD_NODE Datagram);
	void SetDatagramFromPTNSP(PDD_NODE * datagram, PTNSP_NODE PTNSP, SOCKADDR_IN * tempAddr);

	void PrintMessage(PDD_NODE datagram);
	void PrintPTNSP(PTNSP_NODE PTNSP);
	void InputDummy(TNSTable * TNSPTable);
	void InitialFrontEndServer();
}

#endif
