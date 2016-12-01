#ifndef RESOURCELIMITSQOSPOLICY_H_
#define RESOURCELIMITSQOSPOLICY_H_

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
	class CNU_DDS_DLL_API ResourceLimitsQosPolicy : public QosPolicy
	{
	public:
		long	max_samples;
		long	max_instances;
		long	max_samples_per_instance;

	public:
		ResourceLimitsQosPolicy(void);
		~ResourceLimitsQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
