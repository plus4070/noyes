#ifndef HISTORYQOSPOLICY_H_
#define HISTORYQOSPOLICY_H_

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
	enum HistoryQosPolicyKind
	{
		KEEP_LAST_HISTORY_QOS,
		KEEP_ALL_HISTORY_QOS
	};

	class CNU_DDS_DLL_API HistoryQosPolicy : public QosPolicy
	{
	public:
		HistoryQosPolicyKind	kind;
		long					depth;

	public:
		HistoryQosPolicy(void);
		~HistoryQosPolicy(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
