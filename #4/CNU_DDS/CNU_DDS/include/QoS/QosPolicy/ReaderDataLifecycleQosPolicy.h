#ifndef READERDATALIFECYCLEQOSPOLICY_H_
#define READERDATALIFECYCLEQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

namespace CNU_DDS
{
	class ReaderDataLifecycleQosPolicy : public QosPolicy
	{
	public:
		Duration_t	auto_purge_no_writer_samples_delay;
		Duration_t	auto_purge_disposed_samples_delay;

	public:
		ReaderDataLifecycleQosPolicy(void);
		~ReaderDataLifecycleQosPolicy(void);
	};
}

#endif
