#ifndef GROUPDATAQOSPOLICY_H_
#define GROUPDATAQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	const unsigned long	MAX_SIZE_OF_GROUP_DATA_QOS_POLICY	= 256;

	class GroupDataQosPolicy : public QosPolicy
	{
	public:
		unsigned char	value[MAX_SIZE_OF_GROUP_DATA_QOS_POLICY];

	public:
		GroupDataQosPolicy(void);
		~GroupDataQosPolicy(void);
	};
}

#endif
