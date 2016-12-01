#ifndef RELIABILITYQOSPOLICY_H_
#define RELIABILITYQOSPOLICY_H_

#include "QosPolicy.h"
#include "../../DCPS/Infrastructure/Type/Duration_t.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	enum ReliabilityQosPolicyKind
	{
		BEST_EFFROT_REIABILITY_QOS	= 1,
		RELIABLE_RELIABILITY_QOS	= 2
	};

	class CNU_DDS_DLL_API ReliabilityQosPolicy : public QosPolicy
	{
	public:
		ReliabilityQosPolicyKind	kind;
		Duration_t					max_blocking_time;

	public:
		ReliabilityQosPolicy(void);
		~ReliabilityQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
