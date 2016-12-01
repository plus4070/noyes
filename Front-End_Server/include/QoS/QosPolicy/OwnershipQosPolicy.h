#ifndef OWNERSHIPQOSPOLICY_H_
#define OWNERSHIPQOSPOLICY_H_

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
	enum OwnershipQosPolicyKind
	{
		SHARED_OWNERSHIP_QOS,
		EXCLUSIVE_OWNERSHIP_QOS
	};

	class CNU_DDS_DLL_API OwnershipQosPolicy : public QosPolicy
	{
	public:
		OwnershipQosPolicyKind	kind;

	public:
		OwnershipQosPolicy(void);
		~OwnershipQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
