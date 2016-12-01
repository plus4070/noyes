#ifndef MONITOR_H_
#define MONITOR_H_

#include "../../../include/RTPS/Structure/Type/Locator_t.h"
#include "../../../include/RTPS/Messages/Type/ByteStream.h"
#include "../../../include/Utility/DoublyLinkedList.h"
#include "../../../include/Utility/PlatformOrientedInterface.h"

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
	typedef struct MonitoringData_t
	{
		ByteStream*	data;
		Locator_t*	locator;
	}MonitoringData;

	class CNU_DDS_DLL_API Monitor
	{
	private:
		//DoublyLinkedList<ByteStream>*	reader_data_list;
		//DoublyLinkedList<ByteStream>*	writer_data_list;
		DoublyLinkedList<MonitoringData>*	reader_data_list;
		DoublyLinkedList<MonitoringData>*	writer_data_list;

		CONDITION_TYPE_NATIVE			reader_condition;
		CONDITION_TYPE_NATIVE			writer_condition;

	public:
		~Monitor();

		static Monitor*	getMonitorInstance();

		//void			insertReaderData(ByteStream* a_reader_data);
		//void			insertWriterData(ByteStream* a_writer_data);
		//ByteStream*		popReaderData();
		//ByteStream*		popWriterData();
		void			insertReaderData(ByteStream* a_reader_data, Locator_t* locator);
		void			insertWriterData(ByteStream* a_writer_data, Locator_t* locator);
		MonitoringData*		popReaderData();
		MonitoringData*		popWriterData();
		
		void			waitForReaderData();
		void			waitForWriterData();

	private:
		Monitor();
	};

	static Monitor*	MONITOR_INSTANCE;
}

#ifdef __cplusplus
}
#endif
#endif