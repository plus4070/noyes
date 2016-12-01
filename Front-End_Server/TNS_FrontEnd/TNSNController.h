#include <WinSock2.h>
#include <vector>
#include <deque>
#include "StdAfx.h"
#include "TNSTable.h"
#include "SocketManager.h"
#include "ThreadManager.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Publication/Publisher.h"
#include "DCPS/Publication/DataWriter.h"
#include "DCPS/Subscription/Subscriber.h"
#include "DCPS/Subscription/DataReader.h"
#include "DCPS/Topic/Topic.h"

#include "RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"

#include "Utility/Monitoring/Monitor.h"

using namespace CNU_DDS;

#pragma once
class TNSNController
{
public:
	int								TNS_TYPE;
	SocketManager					* socketManager;
	CRITICAL_SECTION				cs;
	deque<PDD_NODE>					* recvData;
	TNSTable						* TNST;
	Monitor							* monitor;
	DomainParticipant				* domainParticipant;
	int								domainParticipantType;
	HANDLE							FrontEndThread;
	HANDLE							ApplicationThread;
	ThreadManager					* tm;
	

	TNSNController(int startOption);
	~TNSNController();

	void			StartTNSServer();
	void			CloseTNSServer();
	
	void			Initialize();
	void			InitialDDS();

	void			InputMonitoringData(MonitoringData * data, int type);

	void			SetSubscriptionData(PDD_NODE * pn);
	void			SetPublicationData(PDD_NODE * pn);

	void			SetMoniter();
	void			SetDomainParticipant(DomainParticipant	*domainPart, int participantType);
		
	void			CheckMonitering();
	void			SetTNSType(int type);
	
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

