#ifndef CHANGEFROMWRITER_H_
#define CHANGEFROMWRITER_H_

#include "../Structure/CacheChange.h"

#include "Type/ChangeFromWriterStatusKind.h"

namespace CNU_DDS
{
	class ChangeFromWriter
	{
	public:
		ChangeFromWriterStatusKind	status;
		bool						is_relevant;
		pCacheChange				cache_change;
	};

	typedef DoublyLinkedList<ChangeFromWriter>	ChangeFromWriterSeq;
}

#endif
