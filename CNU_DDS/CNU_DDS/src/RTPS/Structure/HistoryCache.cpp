#include "../../../include/RTPS/Structure/HistoryCache.h"

#include <stdio.h>

namespace CNU_DDS
{
	HistoryCache::HistoryCache()
	{
		changes	= new CacheChangeSeq();
	}

	HistoryCache::~HistoryCache()
	{
		delete(changes);
	}

#ifdef SHARED_PTR_CACHE_CHANGE
	void				HistoryCache::add_change(pCacheChange a_change)
	{
		changes->insertAscendingInRear(a_change);
	}

#endif

	void				HistoryCache::add_change(CacheChange* a_change)
	{
#ifdef SHARED_PTR_CACHE_CHANGE
		SHARED_PTR_TYPE_NATIVE<CacheChange>	new_change(a_change);
		changes->insertAscendingInRear(new_change);
#else
		changes->insertAscendingInRear(a_change);
#endif
	}

	void				HistoryCache::remove_change(CacheChange* a_change)
	{
#ifdef SHARED_PTR_CACHE_CHANGE
		SharedPtrNode<CacheChange>*	head_node;
		SharedPtrNode<CacheChange>*	cur_node;
#else
		Node<CacheChange>*	head_node;
		Node<CacheChange>*	cur_node;
#endif

		head_node	= changes->getNodeByIndex(0);
		cur_node	= head_node->rear;	// head

		while(cur_node != head_node)
		{
#ifdef SHARED_PTR_CACHE_CHANGE
			if(cur_node->value.get() == a_change)
			{

				changes->cutNode(cur_node);
				cur_node->value.reset();
				delete(cur_node);
				break;
			}
#else
			if(cur_node->value == a_change)
			{
				changes->cutNode(cur_node);
				delete(cur_node->value);
				delete(cur_node);
				break;
			}
#endif

			cur_node	= cur_node->rear;
		}
	}

	SequenceNumber_t	HistoryCache::get_seq_num_min(void)
	{
		if(changes->getSize() <= 0)
		{
			return SEQUENCE_NUMBER_UNKNOWN;
		}

		return	changes->getFirst()->sequence_number;
	}

	SequenceNumber_t	HistoryCache::get_seq_num_max(void)
	{
		if(changes->getSize() <= 0)
		{
			return SEQUENCE_NUMBER_UNKNOWN;
		}

		return	changes->getLast()->sequence_number;
	}
}
