#ifndef PARTITIONQOSPOLICY_H_
#define PARTITIONQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

namespace CNU_DDS
{
	class PartitionQosPolicy : public QosPolicy
	{
	public:
		StringSeq	partition_name;

	public:
		PartitionQosPolicy(void);
		~PartitionQosPolicy(void);
	};
}

#endif
