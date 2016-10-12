#ifndef STATEFULWRITER_H_
#define STATEFULWRITER_H_

#include "../Structure/Writer.h"

#include "ReaderProxy.h"

namespace CNU_DDS
{
	class StatefulWriter : public Writer
	{
	private:
		THREAD_TYPE_NATIVE				processing_thread;
		bool							is_running;

	public:
		ReaderProxySeq*					matched_readers;

	public:
		StatefulWriter();
		StatefulWriter(GUID_t a_guid, ReliabilityKind_t reliability, bool a_push_mode);
		~StatefulWriter();

		void			matched_reader_add(ReaderProxy* a_reader_proxy);
		void			matched_reader_remove(ReaderProxy* a_reader_proxy);
		ReaderProxy*	matched_reader_lookup(GUID_t a_reader_guid);

		bool			is_acked_by_all(CacheChange* a_change);

		CacheChange*	new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle);

	public:
		static void*	processing(void* arg);
		void			start();
		bool			_insertCacheChangesIntoReaderProxy(unsigned long proxy, SequenceNumber_t seq);

	private:
		void*			_get_transceiver();
	};
}

#endif
