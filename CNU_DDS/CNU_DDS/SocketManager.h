#include "TNSNheader.h"
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
	deque<PDD_NODE>					recvData;

	void			 				SetCriticalSection(CRITICAL_SECTION *criticlaSection);

	void							StartRecevingThread();
	void							CloseRecevingThread();

	SOCKET							GetRecevingSocket(int port, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);

	void							CloseProc(int idx, int * totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	PDD_NODE						ReadProc(int idx, int * strLen, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, sockaddr_in * addr);
	void							AcceptProc(int idx, int *totalSocket, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);
	
	void							SavePacketToDeque(CRITICAL_SECTION * cs, deque<PDD_NODE>	* dq, PDD_NODE * pn, sockaddr_in addr);
	void						 	GetRecevingDEQUE(deque<PDD_NODE> ** dq);
	void							SendPacket(char * TargetAddress, const char * Datagram, int SizeOfDatagram, int port);


private:
	HANDLE							recvThread;

	void							SetUIView();
	SOCKET 							CreateSocket();

	void							SetSocketTargetAddress(SOCKET * s, char * TargetAddress, int port);
	void							BindingSocket(SOCKET servSocket, int port);
	static void 					LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	
	void							InsertSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray, SOCKET s, WSAEVENT Event);
	void							DeleteSocketEvent(int * idx, vector<SOCKET> * SocketArray, vector<WSAEVENT> * EventArray);

};

