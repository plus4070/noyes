#ifndef WRITERPROXY_H_
#define WRITERPROXY_H_

#include "../../Utility/SystemDefinition.h"
#include "../Structure/Type/Locator_t.h"
#include "DataStructure/ChangeFromWriterList.h"

#include "Type/ReaderState.h"

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
	class CNU_DDS_DLL_API WriterProxy
	{
	private:
		ReaderState				waiting_state;
		ReaderState				ready_state;
		bool					is_running;

		unsigned long			ack_count;
		//unsigned long			tmp_count;
		//unsigned long			tmp_create_count;
		//unsigned long			tmp_change_count;
		//FILE*					fp;

	public:
		GUID_t					remote_writer_guid;
		LocatorSeq*				unicast_locator_list;
		LocatorSeq*				multicast_locator_list;
		ChangeFromWriterList*	changes_from_writer;
		unsigned long			related_reader;

	private:
		SequenceNumber_t		recent_available_seq;
		//SequenceNumber_t		last_available_seq;

	public:
		WriterProxy();
		WriterProxy(unsigned long reader_instance);
		WriterProxy(unsigned long reader_instance, unsigned long max_cache_size);
		~WriterProxy();

		SequenceNumber_t	available_changes_max(void);
		void				irrelevant_change_set(SequenceNumber_t a_seq_num);
		void				lost_changes_update(SequenceNumber_t first_available_seq_num);
		SequenceNumberSeq*	missing_changes(void);
		void				missing_changes_update(SequenceNumber_t last_available_seq_num);
		void				received_change_set(SequenceNumber_t a_seq_num);

		void				set_event(ReaderEvent event, unsigned long data);

		void				start();

	private:
		void*				_create_a_acknack_message(EntityId_t* writer_id, SequenceNumber_t base, unsigned long num_bits, SequenceNumberSeq* nack_list);
		void				_create_a_cache_data(void* data_submessage, CacheChange* change);
		ChangeFromWriter*	_create_a_cachechange(void* data_submessage);
		//void				_chk_last_available_seq(SequenceNumber_t seq);

	public:
		static void			_after_hartbeat_response_delay(unsigned long arg);

	};

	typedef DoublyLinkedList<WriterProxy>	WriterProxySeq;
}

#ifdef __cplusplus
}
#endif
#endif
