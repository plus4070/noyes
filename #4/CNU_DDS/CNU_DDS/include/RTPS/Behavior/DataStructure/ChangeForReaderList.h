#ifndef INCLUDE_RTPS_BEHAVIOR_DATASTRUCTURE_CHANGEFORREADERLIST_H_
#define INCLUDE_RTPS_BEHAVIOR_DATASTRUCTURE_CHANGEFORREADERLIST_H_

#include "../ChangeForReader.h"

namespace CNU_DDS
{
	typedef struct	CFRnode_t
	{
		struct CFRnode_t*	prev;
		ChangeForReader*	value;
		struct CFRnode_t*	rear;
	}CFRnode;

	class ChangeForReaderList
	{
	private:
		CFRnode*			CFRlist;

		CFRnode*			CFR_unsent;
		CFRnode*			CFR_unacked;
		CFRnode*			CFR_requested;
		CFRnode*			CFR_acked;
		CFRnode*			CFR_underway;

		SequenceNumber_t	base_number;
		unsigned long		head_index;
		unsigned long		size;
		unsigned long		size_limit;

		MUTEX_TYPE_NATIVE	mutex;

	private:
		void				_changeStatus(CFRnode* node, ChangeForReaderStatusKind kind);

	public:
		ChangeForReaderList(unsigned long limit);
		~ChangeForReaderList();

		bool				insertValue(ChangeForReader* cfr);

		ChangeForReader*	getValueByIndex(unsigned long index);
		ChangeForReader*	getValueBySequenceNumber(SequenceNumber_t seq_num);
		ChangeForReader*	getFirstValueByStatus(ChangeForReaderStatusKind kind);
		unsigned long		getValidSize();
		unsigned long		getLimitSize();
		SequenceNumber_t	getBaseNumber();

		ChangeForReader*	popFirstValueByStatus(ChangeForReaderStatusKind kind);

		bool				changeStatusByIndex(unsigned long index, ChangeForReaderStatusKind kind);
		bool				changeStatusBySequenceNumber(SequenceNumber_t seq_num, ChangeForReaderStatusKind kind);

		bool				removeByIndex(unsigned long index);
		bool				removeBySequenceNumber(SequenceNumber_t seq_num);

		void				refresh();
		bool				refreshByStatus();
	};
}

#endif
