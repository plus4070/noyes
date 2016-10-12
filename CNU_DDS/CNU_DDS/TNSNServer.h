#ifndef APPLICATION_RECEIVINGTEST_H_
#define APPLICATION_RECEIVINGTEST_H_

#include "TNSNheader.h"

#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "include/Utility/Monitoring/Monitor.h"

#include "include/DCPS/Publication/DataWriter.h"
#include "include/DCPS/Domain/DomainParticipant.h"
#include "include/DCPS/Publication/Publisher.h"
#include "application/DCPS/ShapeTest.h"

using namespace std;

namespace CNU_DDS
{
	void	StartTopicNamingService(int startOption);
	void	EndTopicNamingService();
	void	InitialDomainForTNS();
	void	SetDomainParticipant(DomainParticipant * dp, int participantType);
}

#endif
