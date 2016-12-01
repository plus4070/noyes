#ifndef QOSPOLICYCOUNT_H_
#define QOSPOLICYCOUNT_H_

#include "../Type/QosPolicyId_t.h"
#include "../../Utility/DoublyLinkedList.h"

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
	class CNU_DDS_DLL_API QosPolicyCount
	{
	public:
		QosPolicyId_t	policy_id;
		long			count;
	};

	typedef DoublyLinkedList<QosPolicyCount>	QosPolicyCountSeq;
}

#ifdef __cplusplus
}
#endif
#endif
