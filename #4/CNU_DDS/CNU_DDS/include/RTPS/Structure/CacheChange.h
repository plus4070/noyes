#ifndef CACHECHANGE_H_
#define CACHECHANGE_H_

#include "../../Utility/SystemDefinition.h"
#include "../../Utility/DoublyLinkedList.h"
#include "../../Utility/SharedPtrList.h"

#include "Type/GUID_t.h"
#include "Type/ChangeKind_t.h"
#include "Type/InstanceHandle_t.h"
#include "Type/SequenceNumber_t.h"
#include "Type/DDS_Data.h"
#include "../Messages/Type/ByteStream.h"

namespace CNU_DDS
{
	class CacheChange
	{
	public:	/* Specification */
		ChangeKind_t		kind;
		GUID_t				writer_guid;
		InstanceHandle_t	instance_handle;
		SequenceNumber_t	sequence_number;
		ByteStream*			data_value;

	public:	/* Extends */
		bool 			operator	>	(const CacheChange& arg);
		bool 			operator	<	(const CacheChange& arg);
		void			operator	=	(const CacheChange& arg);
		bool 			operator	>=	(const CacheChange& arg);
		bool 			operator	<=	(const CacheChange& arg);
		bool 			operator	==	(const CacheChange& arg);
	};

#ifdef SHARED_PTR_CACHE_CHANGE
	typedef SHARED_PTR_TYPE_NATIVE<CacheChange>	pCacheChange;
	typedef SharedPtrList<CacheChange>			CacheChangeSeq;
#else
	typedef CacheChange*						pCacheChange;
	typedef DoublyLinkedList<CacheChange>		CacheChangeSeq;
#endif
}

#endif
