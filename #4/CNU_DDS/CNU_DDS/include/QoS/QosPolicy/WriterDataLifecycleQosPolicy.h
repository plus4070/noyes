#ifndef WRITERDATALIFECYCLEQOSPOLICY_H_
#define WRITERDATALIFECYCLEQOSPOLICY_H_

#include "QosPolicy.h"

namespace CNU_DDS
{
	class WriterDataLifecycleQosPolicy : public QosPolicy
	{
	public:
		bool	auto_dispose_unregistered_instances;

	public:
		WriterDataLifecycleQosPolicy(void);
		~WriterDataLifecycleQosPolicy(void);
	};
}

#endif
