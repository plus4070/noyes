#ifndef DURABILITYQOSPOLICY_H_
#define DURABILITYQOSPOLICY_H_

#include "QosPolicy.h"

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
	enum DurabilityQosPolicyKind
	{
		VOLATILE_DURABILITY_QOS,
		TRANSIENT_LOCAL_DURABILITY_QOS,
		TRANSIENT_DURABILITY_QOS,
		PERSISTENT_DURABILITY_QOS
	};

	class CNU_DDS_DLL_API DurabilityQosPolicy : public QosPolicy
	{
	public:
		DurabilityQosPolicyKind	kind;

	public:
		DurabilityQosPolicy(void);
		~DurabilityQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
