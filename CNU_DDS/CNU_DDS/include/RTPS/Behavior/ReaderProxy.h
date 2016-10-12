#ifndef READERPROXY_H_
#define READERPROXY_H_

#include "../../Utility/SystemDefinition.h"
#include "../Structure/Type/Locator_t.h"
#include "DataStructure/ChangeForReaderList.h"

#include "Type/WriterState.h"
#include "ChangeForReader.h"

namespace CNU_DDS
{
	class ReaderProxy
	{
	private:
		WriterState				pushing_state;
		WriterState				repairing_state;
		bool					is_running;
		bool					wait_a_heartbeating;

		unsigned long			heartbeat_count;
		//unsigned long			tmp_count;

	public:
		GUID_t					remote_reader_guid;
		LocatorSeq*				unicast_locator_list;
		LocatorSeq*				multicast_locator_list;
		ChangeForReaderList*	changes_for_reader;
		bool					expects_inline_qos;
		bool					is_active;
		unsigned long			related_writer;

	public:
		ReaderProxy();
		ReaderProxy(unsigned long writer_instance);
		ReaderProxy(unsigned long writer_instance, unsigned long max_cache_size);
		~ReaderProxy();

		void				acked_changes_set(SequenceNumber_t committed_seq_num);

		ChangeForReader*	next_requested_change(void);
		ChangeForReader*	next_unsent_change(void);
/*
		ChangeForReaderSeq*	unsent_changes(void);
		ChangeForReaderSeq*	requested_changes(void);
		ChangeForReaderSeq*	unacked_changes(void);
*/
		bool				is_acked(SequenceNumber_t seq_num);

		void				requested_changes_set(SequenceNumberSeq* req_seq_num_set);

		void				set_event(WriterEvent event, unsigned long data);

		void				start();

	private:
		void*				_create_a_data_message(EntityId_t* reader_id, CacheChange* change);
		void*				_create_a_heartbeat_message(EntityId_t* reader_id, char flags);

	public:
		static void			_after_heartbeat_period(unsigned long arg);
		static void			_after_nack_response_delay(unsigned long arg);
		static void			_after_nack_suppression_duration(unsigned long arg);
		void				_send_a_heartbeat_message();

		bool				refillChangesForReader();
	};

	typedef DoublyLinkedList<ReaderProxy>	ReaderProxySeq;
}

#endif
