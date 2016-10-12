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
	int								TNS_TYPE;
	SocketManager					* socketManager;
	MessageHandler					* messageHandler;
	CRITICAL_SECTION				cs;
	deque<PDD_NODE>					* recvData;
	TNSTable						* TNST;
	Monitor							* monitor;
	DomainParticipant				* domainParticipant;
	int								domainParticipantType;
	HANDLE							FrontEndService;
	
	TNSNController(int startOption);
	~TNSNController();

	void			StartTNSServer();
	void			CloseTNSServer();
	
	void			Initialize();
	void			InitialDDS();
		
	void			InputDummy(TNSTable * TNSPTable);

	void			CollectDDSParticipant();
	void			InputMonitoringData(MonitoringData * data, int type);

	void			GetSubscriptionData(Monitor * m);
	void			GetPublicationData(Monitor	* m);
	void			SetSubscriptionData(PDD_NODE * pn);
	void			SetPublicationData(PDD_NODE * pn);

	void			SetMoniter();
	void			SetDomainParticipant(DomainParticipant	*domainPart, int participantType);
		
	void			CheckMonitering();
	void			SetTNSType(int type);
	
	void			DistibuteTNSData();
	bool			IsReceviedDataExist();
	bool			IsParticipantDataExist();
	void			InputDummyDataToDB();

	void			StartTNSServiceForFrontEndServer();
	
	void			ProcTerminalPacket(PDD_NODE *datagram);
	void			ProcBackEndPacket(PDD_NODE *datagram);
	void			ProcFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data);


	SOCKADDR_IN		GetAddr(char * cp);
	void			FormationFrontEndPacket(PDD_NODE *datagram, TNSP_ENTRY *data, SOCKADDR_IN *addr);

	void			PrintDatagram(PDD_NODE *datagram, int type);

	void			setAddr(char addr[16], PDD_NODE *datagram);
};

