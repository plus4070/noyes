#ifndef INCLUDE_RTPS_BEHAVIOR_DATASTRUCTURE_CHANGEFROMWRITERLIST_H_
#define INCLUDE_RTPS_BEHAVIOR_DATASTRUCTURE_CHANGEFROMWRITERLIST_H_

#include "../ChangeFromWriter.h"

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
	typedef struct	CFWnode_t
	{
		struct CFWnode_t*	prev;
		ChangeFromWriter*	value;
		struct CFWnode_t*	rear;
	}CFWnode;

	class CNU_DDS_DLL_API ChangeFromWriterList
	{
	private:
		CFWnode*			CFWlist;

		CFWnode*			CFW_lost;
		CFWnode*			CFW_missing;
		CFWnode*			CFW_received;
		CFWnode*			CFW_unknown;

		SequenceNumber_t	base_number;
		unsigned long		head_index;
		unsigned long		size;
		unsigned long		size_limit;

		MUTEX_TYPE_NATIVE	mutex;

	private:
		void				_changeStatus(CFWnode* node, ChangeFromWriterStatusKind kind);

	public:
		ChangeFromWriterList(unsigned long limit);
		~ChangeFromWriterList();

		bool				insertValue(ChangeFromWriter* cfw);

		ChangeFromWriter*	getValueByIndex(unsigned long index);
		ChangeFromWriter*	getValueBySequenceNumber(SequenceNumber_t seq_num);
		ChangeFromWriter*	getFirstValueByStatus(ChangeFromWriterStatusKind kind);
		unsigned long		getValidSize();
		unsigned long		getLimitSize();
		SequenceNumber_t	getBaseNumber();

		SequenceNumberSeq*	getSeqNumListByStatus(ChangeFromWriterStatusKind kind);

		ChangeFromWriter*	popFirstValueByStatus(ChangeFromWriterStatusKind kind);

		bool				changeStatusByIndex(unsigned long index, ChangeFromWriterStatusKind kind);
		bool				changeStatusBySequenceNumber(SequenceNumber_t seq_num, ChangeFromWriterStatusKind kind);

		bool				removeByIndex(unsigned long index);
		bool				removeBySequenceNumber(SequenceNumber_t seq_num);

		void				refresh();
		bool				refreshByStatus();
	};
}

#ifdef __cplusplus
}
#endif
#endif
