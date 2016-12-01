#ifndef CHANGEFROMWRITER_H_
#define CHANGEFROMWRITER_H_

#include "../Structure/CacheChange.h"

#include "Type/ChangeFromWriterStatusKind.h"

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
	class CNU_DDS_DLL_API ChangeFromWriter
	{
	public:
		ChangeFromWriterStatusKind	status;
		bool						is_relevant;
		pCacheChange				cache_change;
	};

	typedef DoublyLinkedList<ChangeFromWriter>	ChangeFromWriterSeq;
}

#ifdef __cplusplus
}
#endif
#endif
