#ifndef APPLICATION_TNSFRONTENDSERVER_H_
#define APPLICATION_TNSFRONTENDSERVER_H_

#include "StdAfx.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "Utility/Monitoring/Monitor.h"

#include "DCPS/Publication/DataWriter.h"
#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Publication/Publisher.h"

using namespace std;

namespace CNU_DDS
{
	void	StartTopicNamingService(int startOption);
	void	SetDomainParticipant(DomainParticipant * dp, int participantType);
}

#endif
