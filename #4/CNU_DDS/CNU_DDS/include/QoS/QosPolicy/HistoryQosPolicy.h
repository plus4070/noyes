#ifndef HISTORYQOSPOLICY_H_
#define HISTORYQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	enum HistoryQosPolicyKind
	{
		KEEP_LAST_HISTORY_QOS,
		KEEP_ALL_HISTORY_QOS
	};

	class HistoryQosPolicy : public QosPolicy
	{
	public:
		HistoryQosPolicyKind	kind;
		long					depth;

	public:
		HistoryQosPolicy(void);
		~HistoryQosPolicy(void);
	};
}

#endif
