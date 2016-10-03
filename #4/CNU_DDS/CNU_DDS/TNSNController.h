#include <WinSock2.h>
#include <vector>
#include <deque>
#include "TNSNheader.h"
#include "TNSTable.h"
#include "MessageHandler.h"
#include "SocketManager.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "include/Utility/Monitoring/Monitor.h"


using namespace CNU_DDS;

#pragma once
class TNSNController
{
public:

	TNSNController();
	~TNSNController();

	void	startTNSServer();
	void	closeTNSServer();
	
	void	initalizeSetting();
	void	initalizeTNSServer();
	void	initalizeServer();

	SOCKET	CreateSocket();
	void	BindingSocket(SOCKET servSocket);
	void	LinkingEvents(SOCKET servSock, int* sockNum, vector<SOCKET> * sockArray, vector<WSAEVENT> * eventArray);
	void	StartReceving();
	void	endServer();

	static UINT WINAPI receiving(LPVOID p);
	static UINT WINAPI storing(LPVOID p);


	void inputDummy(TNSTable * TNSPTable);
	void initalFrontEndServer();

	void GetSubscriptionData(Monitor * m);
	void GetPublicationData(Monitor	* m);
	void SetSubscriptionData(DomainParticipant	*domainPart);
	void SetPublicationData(DomainParticipant *domainPart);


	void setMoniter(Monitor * m);
	void setDomainParticipant(DomainParticipant	*domainPart);

	void checkMonitering();
	void setTNSType(int type);
	

private:
	int								TNS_TYPE;
	SocketManager					* socketManager;
	MessageHandler					* messageHandler;
	CRITICAL_SECTION				cs;
	deque<PDD_NODE>					* recvData;
	TNSTable						* TNST;
	Monitor							* monitor;
	DomainParticipant				* domainParticipant;

	void			distibuteTNSData();
	bool			isReceviedDataExist();
	bool			isParticipantDataExist();
	void			inputDummyDataToDB();
	void			DoTNSService();
	void			CollectDDSParticipant();
	
	void			ProcTerminalPacket(PDD_NODE *datagram);
	void			ProcBackEndPacket(PDD_NODE *datagram);
	void			ProcFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data);

	SOCKADDR_IN		getAddr(char * cp);
	void			formationFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data, SOCKADDR_IN *addr);
};

