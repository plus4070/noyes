#ifndef DESTINATIONORDERQOSPOLICY_H_
#define DESTINATIONORDERQOSPOLICY_H_

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
	enum DestinationOrderQosPolicyKind
	{
		BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS,
		BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS
	};

	class CNU_DDS_DLL_API DestinationOrderQosPolicy : public QosPolicy
	{
	public:
		DestinationOrderQosPolicyKind	kind;

	public:
		DestinationOrderQosPolicy(void);
		~DestinationOrderQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
