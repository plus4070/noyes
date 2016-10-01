#include "../../../include/RTPS/Behavior/StatefulReader.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessage.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"
#include "../../../include/Utility/Network/TransceiverManager.h"

#include "../../../include/DCPS/Subscription/DataReader.h"
#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"

#include <stdio.h>

namespace CNU_DDS
{
	StatefulReader::StatefulReader()
	{
		matched_writers	= new WriterProxySeq();
		is_running		= false;
	}

	StatefulReader::StatefulReader(GUID_t a_guid, ReliabilityKind_t reliability)
	{
		this->guid.guid_prefix		= a_guid.guid_prefix;
		this->guid.entity_id		= a_guid.entity_id;
		this->reliability_level		= reliability;
		this->is_running			= false;

		matched_writers	= new WriterProxySeq();
	}

	StatefulReader::~StatefulReader()
	{
		delete(matched_writers);
		destroy_thread(&processing_thread);
	}

	void			StatefulReader::start()
	{
		if(is_running == false)
		{
			is_running	= true;
			create_thread(&processing_thread, StatefulReader::processing, this);
		}
	}

	void			StatefulReader::matched_writer_add(WriterProxy* a_writer_proxy)
	{
		matched_writers->insertInRear(a_writer_proxy);
	}

	void			StatefulReader::matched_writer_remove(WriterProxy* a_writer_proxy)
	{
		Node<WriterProxy>*	head		= matched_writers->getNodeByIndex(0);
		Node<WriterProxy>*	cur_node	= head->rear;

		matched_writers->listLock();
		while(cur_node != head)
		{
			if(cur_node->value == a_writer_proxy)
			{
				matched_writers->listUnlock();
				matched_writers->cutNode(cur_node);
				matched_writers->listLock();
				delete(a_writer_proxy);
				break;
			}

			cur_node	= cur_node->rear;
		}
		matched_writers->listUnlock();
	}

	WriterProxy*	StatefulReader::matched_writer_lookup(GUID_t a_writer_guid)
	{
		WriterProxy*		result		= NULL;
		Node<WriterProxy>*	head		= matched_writers->getNodeByIndex(0);
		Node<WriterProxy>*	cur_node	= head->rear;

		matched_writers->listLock();
		while(cur_node != head)
		{
			if(cur_node->value->remote_writer_guid == a_writer_guid)
			{
				result	= cur_node->value;
			}

			cur_node	= cur_node->rear;
		}
		matched_writers->listUnlock();

		return	result;
	}

	void*			StatefulReader::_get_transceiver()
	{
		TransceiverManager*	transceiver_manager	= TransceiverManager::getManagerInstance();
		Transceiver*		transceiver;
		unsigned long		transceiver_id;
		unsigned long		domain_id	= this->related_dcps_reader->related_participant->get_domain_id();

		switch(this->guid.entity_id.entity_kind)
		{
		case ENTITY_KIND_BUILTIN_WRITER_WITH_KEY:
		case ENTITY_KIND_BUILTIN_WRITER_NO_KEY:
		case ENTITY_KIND_BUILTIN_READER_NO_KEY:
		case ENTITY_KIND_BUILTIN_READER_WITH_KEY:
			transceiver_id	= TRANSCEIVER_ID_METATRAFFIC_UNICAST + ( domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN );
			break;
		default:
			transceiver_id	= TRANSCEIVER_ID_DEFAULT_UNICAST + ( domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN );
			break;
		}

		transceiver	= transceiver_manager->getTransceiverInstance(transceiver_id);

		return transceiver;
	}

	EntityId_t*		StatefulReader::_get_writer_entity_id(void* submessage)
	{
		EntityId_t*	result;

		if(submessage == NULL)
		{
			return NULL;
		}

		switch(((Submessage*)submessage)->submessage_header.submessage_id)
		{
		case SUBMESSAGE_KIND_DATA:
			result	= &((Data*)submessage)->writer_id.value;
			break;

		case SUBMESSAGE_KIND_ACKNACK:
			result	= &((AckNack*)submessage)->writer_id.value;
			break;

		case SUBMESSAGE_KIND_HEARTBEAT:
			result	= &((Heartbeat*)submessage)->writer_id.value;
			break;

		case SUBMESSAGE_KIND_GAP:
			result	= &((Gap*)submessage)->writer_id.value;
			break;

		default:
			result	= NULL;
			break;
		}

		return result;
	}

	void*	StatefulReader::processing(void* arg)
	{
		StatefulReader*	reader		= (StatefulReader*)arg;
		EntityEvent*	event		= NULL;
		Message*		message		= NULL;
		Submessage*		submessage	= NULL;
		WriterProxy*	proxy		= NULL;
		Transceiver*	transceiver	= NULL;
		int				i			= 0;
		bool			is_useful	= false;
		GUID_t			writer_guid;
		EntityId_t*		p_entity_id;

		transceiver	= (Transceiver*)reader->_get_transceiver();

		while(reader->is_running)
		{
			while(reader->events->getSize() > 0)
			{
				//printf("event popping\n");
				event	= reader->events->popFirst();

				switch((ReaderEvent)event->event)
				{
				case SEND_A_MESSAGE_TO_WRITER:
					if(transceiver)
					{
						proxy	= (WriterProxy*)event->target;
						transceiver->send((ByteStream*)event->data, *proxy->unicast_locator_list->getFirst());
						delete(event);
					}
					else
					{
						transceiver	= (Transceiver*)reader->_get_transceiver();
						reader->SetEvent(event);
					}
					break;

				case DATA_IS_RECEIVED:
				case HEARTBEAT_IS_RECEIVED:
				case GAP_IS_RECEIVED:
//printf("StatefulReader::receive something\n");
					message					= (Message*)event->data;
					if(message == NULL)
					{
						printf("\nERROR: event->data is NULL\n");
						break;
					}
					writer_guid.guid_prefix	= message->header.guid_prefix;
					p_entity_id				= (reader->_get_writer_entity_id((void*)(message->submessages->getFirst())));
					if(p_entity_id == NULL)
					{
						printf("\nERROR: received EntityId is wrong\n");
						break;
					}
					writer_guid.entity_id	= *p_entity_id;

					is_useful				= false;

//printf("matched_writers(%d)\n", reader->matched_writers->getSize());
					for(i=1; i<=reader->matched_writers->getSize(); i++)
					{
						proxy		= reader->matched_writers->getNodeByIndex(i)->value;
//printf("getWriterProxy(%08X)(%08X, %08X, %08X)(%08X))\n", proxy, *((unsigned long*)proxy->remote_writer_guid.guid_prefix.host_id), *((unsigned long*)proxy->remote_writer_guid.guid_prefix.app_id), *((unsigned long*)proxy->remote_writer_guid.guid_prefix.counter), *((unsigned long*)&proxy->remote_writer_guid.entity_id));
//printf("writer_guid         (%08X, %08X, %08X)(%08X)\n", *((unsigned long*)writer_guid.guid_prefix.host_id), *((unsigned long*)writer_guid.guid_prefix.app_id), *((unsigned long*)writer_guid.guid_prefix.counter), *((unsigned long*)&writer_guid.entity_id));
						if(proxy != NULL && proxy->remote_writer_guid == writer_guid)
						{
//printf("find_related_WriterProxy\n");
							proxy->set_event((ReaderEvent)event->event, event->data);
							is_useful	= true;
							break;
						}
						else
						{
//printf("else\n");
						}
					}

					if(is_useful == false)
					{
//printf("\nStatefulReader::processing() Event is useless(e:%d)(t:%d)(d:%d)\n", event->event, event->target, event->data);
						message->submessages->deleteAll();
						delete(message);
						delete(event);
					}
					break;

				default:
//printf("StatefulReader sets a event to a proxy\n");
					proxy	= (WriterProxy*)event->target;
					proxy->set_event((ReaderEvent)event->event, event->data);
					delete(event);
					break;
				}
//printf("clear\n");
			}

			reader->Wait();
		}

		return 0;
	}
}
