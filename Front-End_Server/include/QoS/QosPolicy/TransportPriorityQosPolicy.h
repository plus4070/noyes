#ifndef TRANSPORTPRIORITYQOSPOLICY_H_
#define TRANSPORTPRIORITYQOSPOLICY_H_

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
	class CNU_DDS_DLL_API TransportPriorityQosPolicy : public QosPolicy
	{
	public:
		long	value;

	public:
		TransportPriorityQosPolicy(void);
		~TransportPriorityQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
