#ifndef PRESENTATIONQOSPOLICY_H_
#define PRESENTATIONQOSPOLICY_H_

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
	enum PresentationQosPolicyAccessScopeKind
	{
		INSTANCE_PRESENTATION_QOS,
		TOPIC_PRESENTATION_QOS,
		GROUP_PRESENTATION_QOS
	};

	class CNU_DDS_DLL_API PresentationQosPolicy : public QosPolicy
	{
	public:
		PresentationQosPolicyAccessScopeKind	access_scope;
		bool									coherent_access;
		bool									ordered_access;

	public:
		PresentationQosPolicy(void);
		~PresentationQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
