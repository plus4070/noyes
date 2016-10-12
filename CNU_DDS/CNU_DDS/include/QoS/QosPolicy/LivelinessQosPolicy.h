#ifndef LIVELINESSQOSPOLICY_H_
#define LIVELINESSQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	enum LivelinessQosPolicyKind
	{
		AUTOMATIC_LIVELINESS_QOS,
		MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
		MANUAL_BY_TOPIC_LIVELINESS_QOS
	};

	class LivelinessQosPolicy : public QosPolicy
	{
	public:
		LivelinessQosPolicyKind	kind;
		Duration_t				lease_duration;

	public:
		LivelinessQosPolicy(void);
		~LivelinessQosPolicy(void);
	};
}

#endif
