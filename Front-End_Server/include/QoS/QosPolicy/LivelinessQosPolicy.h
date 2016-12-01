#ifndef LIVELINESSQOSPOLICY_H_
#define LIVELINESSQOSPOLICY_H_

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
	enum LivelinessQosPolicyKind
	{
		AUTOMATIC_LIVELINESS_QOS,
		MANUAL_BY_PARTICIPANT_LIVELINESS_QOS,
		MANUAL_BY_TOPIC_LIVELINESS_QOS
	};

	class CNU_DDS_DLL_API LivelinessQosPolicy : public QosPolicy
	{
	public:
		LivelinessQosPolicyKind	kind;
		Duration_t				lease_duration;

	public:
		LivelinessQosPolicy(void);
		~LivelinessQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
