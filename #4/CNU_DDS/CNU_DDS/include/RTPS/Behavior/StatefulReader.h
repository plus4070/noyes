#ifndef STATEFULREADER_H_
#define STATEFULREADER_H_

#include "../Structure/Reader.h"

#include "WriterProxy.h"

namespace CNU_DDS
{
	class StatefulReader : public Reader
	{
	private:
		THREAD_TYPE_NATIVE				processing_thread;
		bool							is_running;

	public:
		WriterProxySeq*					matched_writers;

	public:
		StatefulReader();
		StatefulReader(GUID_t a_guid, ReliabilityKind_t reliability);
		~StatefulReader();

		void			matched_writer_add(WriterProxy* a_writer_proxy);
		void			matched_writer_remove(WriterProxy* a_writer_proxy);
		WriterProxy*	matched_writer_lookup(GUID_t a_writer_guid);

	public:
		static void*	processing(void* arg);
		void			start();

	//private:
	public:
		void*			_get_transceiver();
		EntityId_t*		_get_writer_entity_id(void* submessage);
	};
}

#endif
