#include "stdafx.h"
#include "TopicNameTable.h"
#include <process.h>
#include <WinSock2.h>
#include <vector>
#include <deque>

using namespace std;

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma once

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	CRITICAL_SECTION				cs;
	deque<pair<IN_ADDR, PDD_NODE>>	recvData;
	TopicNameTable *TNTable;

	void	SetTNTable(TopicNameTable* TNTable);
	void	SetCriticalSection(CRITICAL_SECTION *criticlaSection);

	//Receiving
	void	StartRecevingThread();
	void	CloseRecevingThread();
	void	GetRecevingDEQUE(deque<pair<IN_ADDR, PDD_NODE>> ** dq);
	SOCKET	GetRecevingSocket(int port, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);

	//Sending
	void	StartSendingThread();
	void	CloseSendingThread();

	PDD_NODE	ReadProc(int idx, int * strLen, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, sockaddr_in * addr);
	void	CloseProc(int idx, int * totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	void	AcceptProc(int idx, int *totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	void	SavePacketToDeque(CRITICAL_SECTION * cs, deque<pair<IN_ADDR, PDD_NODE>>	* dq, PDD_NODE * pn, sockaddr_in addr);
	void	SendPacket(char * TargetAddress, const char * Datagram, int SizeOfDatagram, int port);

private:
	HANDLE	recvThread;
	HANDLE	sendThread;

	void	SetUIView();
	SOCKET	CreateSocket();

	void	SetSocketTargetAddress(SOCKET * s, char * TargetAddress, int port);
	void	BindingSocket(SOCKET servSocket, int PORT);
	static void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);

	//receving
	bool	IsWsaWaitERROR();

	bool	AcceptConnection();
	bool	CloseConnection();

	void	InsertSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, SOCKET s, WSAEVENT Event);
	void	DeleteSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
};