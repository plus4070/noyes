#include "../../../include/RTPS/Behavior/StatelessReader.h"
#include "../../../include/RTPS/Behavior/ListenerProcessor.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessage.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"

#include <stdio.h>

namespace CNU_DDS
{
	StatelessReader::StatelessReader()
	{
		this->is_running	= false;

	}

	StatelessReader::StatelessReader(GUID_t a_guid, ReliabilityKind_t reliability)
	{
		this->guid.entity_id		= a_guid.entity_id;
		this->guid.guid_prefix		= a_guid.guid_prefix;
		this->reliability_level		= reliability;
		this->is_running			= false;
	}

	StatelessReader::~StatelessReader()
	{
		is_running	= false;
		destroy_thread(&processing_thread);
	}

	void			StatelessReader::start()
	{
		if(is_running == false)
		{
			is_running	= true;
			create_thread(&processing_thread, StatelessReader::processing, this);
		}
	}

	void*	StatelessReader::processing(void* arg)
	{
		StatelessReader*	reader	= (StatelessReader*)arg;
		EntityEvent*		event;
		CacheChange*		new_change;
		Message*			message;
		Submessage*			submessage;
		Data*				data;
		ListenerProcessor*	processor		= ListenerProcessor::getListenerProcessorInstance();
		Event*				listener_event	= NULL;
		int					i;

		while(reader->is_running)
		{
			while(reader->events->getSize() > 0)
			{
				event	= reader->events->popFirst();
				message	= (Message*)event->data;

				for(i=1; i<=message->submessages->getSize(); i++)
				{
					submessage	= message->submessages->getNodeByIndex(i)->value;

					if(submessage->submessage_header.submessage_id == SUBMESSAGE_KIND_DATA)
					{
						data		= (Data*)submessage;
						new_change	= new CacheChange();
						new_change->sequence_number				= data->writer_sn.value;
						new_change->writer_guid.guid_prefix		= message->header.guid_prefix;
						new_change->writer_guid.entity_id		= data->writer_id.value;
						new_change->kind						= CHANGE_KIND_ALIVE;
						new_change->instance_handle				= 0; //????
						new_change->data_value					= new ByteStream();

						if(!memcmp(data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_BE, 4))
						{
							new_change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN, data->serialized_payload.value.getData(), data->serialized_payload.value.getLength());
						}
						else if(!memcmp(data->serialized_payload.info, SERIALIZED_DATA_INFO_PL_CDR_BE, 4))
						{
							new_change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST, data->serialized_payload.value.getData(), data->serialized_payload.value.getLength());
						}
						else if(!memcmp(data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_LE, 4))
						{
							new_change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, data->serialized_payload.value.getData(), data->serialized_payload.value.getLength());
						}
						else
						{
							new_change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST, data->serialized_payload.value.getData(), data->serialized_payload.value.getLength());
						}

						reader->reader_cache->add_change(new_change);

						listener_event	= new Event();
						listener_event->type	= EVENT_DATA_AVAILABLE;
						listener_event->arg		= (unsigned long)reader->related_dcps_reader;
						processor->setEvent(listener_event);

						break;
					}
				}

				delete(message);
				delete(event);
			}

			reader->Wait();
		}

		return 0;
	}
}
