#ifndef TIMEBASEDFILTERQOSPOLICY_H_
#define TIMEBASEDFILTERQOSPOLICY_H_

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
	class CNU_DDS_DLL_API TimeBasedFilterQosPolicy : public QosPolicy
	{
	public:
		Duration_t	minimum_separation;

	public:
		TimeBasedFilterQosPolicy(void);
		~TimeBasedFilterQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
