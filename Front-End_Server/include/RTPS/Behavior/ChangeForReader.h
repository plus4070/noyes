#ifndef CHANGEFORREADER_H_
#define CHANGEFORREADER_H_

#include "Type/ChangeForReaderStatusKind.h"

#include "../Structure/CacheChange.h"

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
	class CNU_DDS_DLL_API ChangeForReader
	{
	public:
		ChangeForReaderStatusKind	status;
		bool						is_relevant;
		pCacheChange				cache_change;
	};

	typedef DoublyLinkedList<ChangeForReader>	ChangeForReaderSeq;
}

#ifdef __cplusplus
}
#endif
#endif
