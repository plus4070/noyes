#ifndef CHANGEFORREADER_H_
#define CHANGEFORREADER_H_

#include "Type/ChangeForReaderStatusKind.h"

#include "../Structure/CacheChange.h"

namespace CNU_DDS
{
	class ChangeForReader
	{
	public:
		ChangeForReaderStatusKind	status;
		bool						is_relevant;
		pCacheChange				cache_change;
	};

	typedef DoublyLinkedList<ChangeForReader>	ChangeForReaderSeq;
}

#endif
