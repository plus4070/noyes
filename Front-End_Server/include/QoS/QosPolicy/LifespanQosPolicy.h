#ifndef LIFESPANQOSPOLICY_H_
#define LIFESPANQOSPOLICY_H_

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
	class CNU_DDS_DLL_API LifespanQosPolicy : public QosPolicy
	{
	public:
		Duration_t	duration;

	public:
		LifespanQosPolicy(void);
		~LifespanQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
