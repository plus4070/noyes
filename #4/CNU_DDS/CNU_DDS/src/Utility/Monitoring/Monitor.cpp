#include "../../../include/Utility/Monitoring/Monitor.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"

namespace CNU_DDS
{
	Monitor::Monitor()
	{
		//reader_data_list	= new DoublyLinkedList<ByteStream>();
		//writer_data_list	= new DoublyLinkedList<ByteStream>();
		reader_data_list	= new DoublyLinkedList<MonitoringData>();
		writer_data_list	= new DoublyLinkedList<MonitoringData>();

		create_condition(&reader_condition);
		create_condition(&writer_condition);
	}

	Monitor::~Monitor()
	{
		if(MONITOR_INSTANCE)
		{
			MONITOR_INSTANCE	= NULL;
		}
	}

	Monitor*	Monitor::getMonitorInstance()
	{
		if(MONITOR_INSTANCE)
		{
			// do nothing
		}
		else
		{
			MONITOR_INSTANCE	= new Monitor();
		}
		return MONITOR_INSTANCE;
	}

	//void			Monitor::insertReaderData(ByteStream* a_reader_data)
	void			Monitor::insertReaderData(ByteStream* a_reader_data, Locator_t* locator)
	{
		MonitoringData*	monitoring_data	= new MonitoringData();
		ByteStream*		new_reader_data	= new ByteStream();

		*new_reader_data	= *a_reader_data;

		monitoring_data->data		= new_reader_data;
		monitoring_data->locator	= locator;
		reader_data_list->insertInRear(monitoring_data);
		condition_signal(&reader_condition);
	}

	//void			Monitor::insertWriterData(ByteStream* a_writer_data)
	void			Monitor::insertWriterData(ByteStream* a_writer_data, Locator_t* locator)
	{
		MonitoringData*	monitoring_data	= new MonitoringData();
		ByteStream*	new_writer_data	= new ByteStream();

		*new_writer_data	= *a_writer_data;

		monitoring_data->data		= new_writer_data;
		monitoring_data->locator	= locator;
		writer_data_list->insertInRear(monitoring_data);
		condition_signal(&writer_condition);
	}

	//ByteStream*		Monitor::popReaderData()
	MonitoringData*		Monitor::popReaderData()
	{
		MonitoringData*	data	= NULL;

		if(reader_data_list->getSize() > 0)
		{
			data	= reader_data_list->popFirst();
		}

		return data;
	}

	//ByteStream*		Monitor::popWriterData()
	MonitoringData*		Monitor::popWriterData()
	{
		MonitoringData*	data	= NULL;

		if(writer_data_list->getSize() > 0)
		{
			data	= writer_data_list->popFirst();
		}

		return data;
	}

	void			Monitor::waitForReaderData()
	{
		if(reader_data_list->getSize() == 0)
		{
			condition_wait(&reader_condition, NULL);
		}
	}

	void			Monitor::waitForWriterData()
	{
		if(writer_data_list->getSize() == 0)
		{
			condition_wait(&writer_condition, NULL);
		}
	}
}