#ifndef HISTORYCACHE_H_
#define HISTORYCACHE_H_

#include "CacheChange.h"

namespace CNU_DDS
{
	class HistoryCache
	{
	public:
		CacheChangeSeq*	changes;

	public:
		HistoryCache();
		~HistoryCache();

#ifdef SHARED_PTR_CACHE_CHANGE
		void				add_change(pCacheChange a_change);
		void				add_change(CacheChange* a_change);
#else
		void				add_change(CacheChange* a_change);
#endif
		void				remove_change(CacheChange* a_change);

		SequenceNumber_t	get_seq_num_min(void);
		SequenceNumber_t	get_seq_num_max(void);
	};
}

#endif
