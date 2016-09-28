#include "stdafx.h"
#include <process.h>
#include <winSock2.h>
#include <vector>
#include <deque>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma once
class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	CRITICAL_SECTION				cs;
	deque<pair<IN_ADDR, PDD_NODE>>	recvData;

	void			 				setCriticalSection(CRITICAL_SECTION *criticlaSection);

	void							startRecevingThread();
	void							closeRecevingThread();

	SOCKET							getRecevingSocket(int port, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);

	void							CloseProc(int idx, int * totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	PDD_NODE						ReadProc(int idx, int * strLen, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	void							AcceptProc(int idx, int *totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	void							savePacketToDeque(CRITICAL_SECTION * cs, deque<pair<IN_ADDR, PDD_NODE>>	* dq, PDD_NODE pn, sockaddr_in addr);
	void						 	getRecevingDEQUE(deque<pair<IN_ADDR, PDD_NODE>> * dq);
	void							sendPacket(char * TargetAddress, const char * Datagram, int SizeOfDatagram, int port);
private:

	HANDLE							recvThread;
	void							setUIView();


	SOCKET 							createSocket();
	void							setSocketTargetAddress(SOCKET * s, char * TargetAddress, int port);
	void							bindingSocket(SOCKET servSocket, int PORT);
	static void 					linkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	
	//receving
	bool							isWsaWaitERROR();

	bool							acceptConnection();
	bool							closeConnection();

	void							insertSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, SOCKET s, WSAEVENT Event);
	void							deleteSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
};

