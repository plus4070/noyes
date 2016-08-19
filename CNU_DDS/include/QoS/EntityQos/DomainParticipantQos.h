#ifndef DOMAINPARTICIPANTQOS_H_
#define DOMAINPARTICIPANTQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class DomainParticipantQos : public EntityQos
	{
	public:
		UserDataQosPolicy		user_data;
		EntityFactoryQosPolicy	entity_factory;
	};
}

#endif
