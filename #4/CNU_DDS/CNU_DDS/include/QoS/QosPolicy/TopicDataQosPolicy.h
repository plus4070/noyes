#ifndef TOPICDATAQOSPOLICY_H_
#define TOPICDATAQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	const unsigned long	MAX_SIZE_OF_TOPIC_DATA_QOS_POLICY	= 256;

	class TopicDataQosPolicy : public QosPolicy
	{
	public:
		unsigned char	value[MAX_SIZE_OF_TOPIC_DATA_QOS_POLICY];

	public:
		TopicDataQosPolicy(void);
		~TopicDataQosPolicy(void);
	};
}

#endif
