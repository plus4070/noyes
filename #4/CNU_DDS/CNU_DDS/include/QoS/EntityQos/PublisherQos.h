#ifndef PUBLISHERQOS_H_
#define PUBLISHERQOS_H_

#include "EntityQos.h"
#include "../QosPolicies.h"

namespace CNU_DDS
{
	class PublisherQos : public EntityQos
	{
	public:
		PresentationQosPolicy	presentation;
		PartitionQosPolicy		partition;
		GroupDataQosPolicy		group_data;
		EntityFactoryQosPolicy	entity_factory;
	};
}

#endif
