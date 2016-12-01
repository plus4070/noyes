#include "StdAfx.h"
#include "TNSNServer.h"
#include "TNSNController.h"

namespace CNU_DDS
{
	TNSNController		*controller;
	Monitor				*tnsMonitor;
	DomainParticipant	*tnsDomainPart;

	void StartTopicNamingService(int startOption) {
		//initalDomainForTNS();
		controller = new TNSNController(startOption);
	}

	void SetDomainParticipant(DomainParticipant * dp, int participantType) {
		controller->SetDomainParticipant(dp, participantType);
	}
}

