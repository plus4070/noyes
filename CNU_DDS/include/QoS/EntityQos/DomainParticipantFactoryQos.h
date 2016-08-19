#ifndef DOMAINPARTICIPANTFACTORYQOS_H_
#define DOMAINPARTICIPANTFACTORYQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class DomainParticipantFactoryQos : public EntityQos
	{
	public:
		EntityFactoryQosPolicy	entity_factory;
	};
}

#endif
