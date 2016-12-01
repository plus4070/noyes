#ifndef OWNERSHIPSTRENGTHQOSPOLICY_H_
#define OWNERSHIPSTRENGTHQOSPOLICY_H_

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
	class CNU_DDS_DLL_API OwnershipStrengthQosPolicy : public QosPolicy
	{
	public:
		long	value;

	public:
		OwnershipStrengthQosPolicy(void);
		~OwnershipStrengthQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
