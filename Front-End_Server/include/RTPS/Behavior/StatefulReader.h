#ifndef STATEFULREADER_H_
#define STATEFULREADER_H_

#include "../Structure/Reader.h"

#include "WriterProxy.h"

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
	class CNU_DDS_DLL_API StatefulReader : public Reader
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

#ifdef __cplusplus
}
#endif
#endif
