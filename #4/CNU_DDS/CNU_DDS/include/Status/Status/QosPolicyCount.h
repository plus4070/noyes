#ifndef QOSPOLICYCOUNT_H_
#define QOSPOLICYCOUNT_H_

#include "../Type/QosPolicyId_t.h"
#include "../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class QosPolicyCount
	{
	public:
		QosPolicyId_t	policy_id;
		long			count;
	};

	typedef DoublyLinkedList<QosPolicyCount>	QosPolicyCountSeq;
}

#endif
