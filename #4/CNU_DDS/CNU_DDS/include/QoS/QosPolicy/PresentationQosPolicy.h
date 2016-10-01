#ifndef PRESENTATIONQOSPOLICY_H_
#define PRESENTATIONQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	enum PresentationQosPolicyAccessScopeKind
	{
		INSTANCE_PRESENTATION_QOS,
		TOPIC_PRESENTATION_QOS,
		GROUP_PRESENTATION_QOS
	};

	class PresentationQosPolicy : public QosPolicy
	{
	public:
		PresentationQosPolicyAccessScopeKind	access_scope;
		bool									coherent_access;
		bool									ordered_access;

	public:
		PresentationQosPolicy(void);
		~PresentationQosPolicy(void);
	};
}

#endif
