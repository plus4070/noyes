#ifndef SUBSCRIBERQOS_H_
#define SUBSCRIBERQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class SubscriberQos : public EntityQos
	{
	public:
		PresentationQosPolicy	presentation;
		PartitionQosPolicy		partition;
		GroupDataQosPolicy		group_data;
		EntityFactoryQosPolicy	entity_factory;
	};
}

#endif
