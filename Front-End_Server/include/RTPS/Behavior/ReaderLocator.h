#ifndef READERLOCATOR_H_
#define READERLOCATOR_H_

#include "../Structure/Type/Locator_t.h"
#include "../Structure/CacheChange.h"
#include "Type/WriterState.h"

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
	class CNU_DDS_DLL_API ReaderLocator
	{
	private:
		WriterState		pushing_state;
		WriterState		repairing_state;

	public:
		CacheChangeSeq*	requested_changes;
		CacheChangeSeq*	unsent_changes;
		Locator_t		locator;
		bool			expects_inline_qos;
		unsigned long	related_writer;

	public:
		ReaderLocator(unsigned long writer_instance, Locator_t reader_locator, bool a_expects_inline_qos);
		~ReaderLocator();

		pCacheChange	next_requested_change(void);
		pCacheChange	next_unsent_change(void);

		void			requested_changes_set(SequenceNumberSeq* req_seq_num_set);

		void			set_event(WriterEvent event, unsigned long data);

	private:
		void*			_create_a_data_message(CacheChange* change);
	};

	typedef DoublyLinkedList<ReaderLocator>	ReaderLocatorSeq;
}

#ifdef __cplusplus
}
#endif
#endif
