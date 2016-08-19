#ifndef DESTINATIONORDERQOSPOLICY_H_
#define DESTINATIONORDERQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	enum DestinationOrderQosPolicyKind
	{
		BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
		BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
	};

	class DestinationOrderQosPolicy : public QosPolicy
	{
	public:
		DestinationOrderQosPolicyKind	kind;

	public:
		DestinationOrderQosPolicy(void);
		~DestinationOrderQosPolicy(void);
	};
}

#endif
