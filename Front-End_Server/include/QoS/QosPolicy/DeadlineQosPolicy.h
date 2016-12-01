#ifndef DEADLINEQOSPOLICY_H_
#define DEADLINEQOSPOLICY_H_

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
	class CNU_DDS_DLL_API DeadlineQosPolicy: public QosPolicy
	{
	public:
		Duration_t	period;

	public:
		DeadlineQosPolicy(void);
		~DeadlineQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
