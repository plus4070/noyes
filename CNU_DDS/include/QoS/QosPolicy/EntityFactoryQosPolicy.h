#ifndef ENTITYFACTORYQOSPOLICY_H_
#define ENTITYFACTORYQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	class EntityFactoryQosPolicy : public QosPolicy
	{
	public:
		bool	auto_enable_created_entities;

	public:
		EntityFactoryQosPolicy(void);
		~EntityFactoryQosPolicy(void);
	};
}

#endif
