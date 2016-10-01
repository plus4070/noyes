#include "../../../include/RTPS/Behavior/StatefulWriter.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"
#include "../../../include/Utility/Network/TransceiverManager.h"

#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/DataWriter.h"

#include <stdio.h>

namespace CNU_DDS
{
	StatefulWriter::StatefulWriter()
	{
		matched_readers	= new ReaderProxySeq();
		is_running		= false;
	}

	StatefulWriter::StatefulWriter(GUID_t a_guid, ReliabilityKind_t reliability, bool a_push_mode)
	{
		this->guid.entity_id		= a_guid.entity_id;
		this->guid.guid_prefix		= a_guid.guid_prefix;
		this->reliability_level		= reliability;
		this->push_mode				= a_push_mode;
		this->is_running			= false;

		matched_readers	= new ReaderProxySeq();
	}

	StatefulWriter::~StatefulWriter()
	{
		matched_readers->deleteAll();

		destroy_thread(&processing_thread);
		delete(matched_readers);
	}

	void			StatefulWriter::start()
	{
		if(is_running == false)
		{
			//printf("StatefulWriter(%08X)\n", this);
			is_running	= true;
			create_thread(&processing_thread, StatefulWriter::processing, this);
		}
	}

	bool			StatefulWriter::_insertCacheChangesIntoReaderProxy(unsigned long proxy, SequenceNumber_t seq)
	{
		ReaderProxy*		target	= (ReaderProxy*)proxy;
		bool				result	= false;
		SequenceNumber_t	calc_seq;
		int					i			= 0;
		ChangeForReader*	cfr			= NULL;
		Node<CacheChange>*	cur_node	= NULL;
		Node<CacheChange>*	head_node	= NULL;
		EntityEvent*		event		= NULL;

		//printf("StatefulWriter::_insertCacheChangesIntoReaderProxy(%d, %d)\n", seq.high, seq.low);

		if(last_change_sequence_number < seq)
		{
			//printf("\t(false) last_change_sequence_number < seq\n");
			//printf("\tlast_seq_num(%d, %d)\n", last_change_sequence_number.high, last_change_sequence_number.low);
			result	= false;
		}
		else
		{
			//printf("\t(true) last_change_sequence_number >= seq\n");
			if(writer_cache->changes->getSize())
			{
				head_node	= writer_cache->changes->getNodeByIndex(0);
				cur_node	= writer_cache->changes->getNodeByIndex(1);

				writer_cache->changes->listLock();
				while(head_node != cur_node)
				{
					if((cur_node->value != NULL) && (seq <= cur_node->value->sequence_number))
					{
						break;
					}
					cur_node	= cur_node->rear;
				}
				writer_cache->changes->listUnlock();

				if((cur_node != NULL) && (seq <= cur_node->value->sequence_number))
				{
					writer_cache->changes->listLock();
					while(cur_node != head_node)
					{
						result	= true;
						cfr	= new ChangeForReader();
						cfr->is_relevant	= true;
						cfr->cache_change	= cur_node->value;

						//printf("\t\tcfr(%d)\n", cfr->cache_change->sequence_number.low);
						
						if(this->push_mode == true)
						{
							cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
						}
						else
						{
							cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
						}

						if(!target->changes_for_reader->insertValue(cfr))
						{
							printf("\t\tinsertValue is failed(%d)\n", cfr->cache_change->sequence_number.low);
							delete(cfr);
							break;
						}
						cur_node	= cur_node->rear;
					}
					writer_cache->changes->listUnlock();
				}
			}

			return result;
			/*
			if(writer_cache->changes->getSize())
			{
				calc_seq	= seq	- writer_cache->changes->getFirst()->sequence_number;

				for(i=1; i<=calc_seq.low+1; i++)
				{
					cfr	= new ChangeForReader();
					cfr->is_relevant	= true;

					cfr->cache_change	= writer_cache->changes->getNodeByIndex(i)->value;

					if(this->push_mode == true)
					{
						cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
					}
					else
					{
						cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
					}

					if((cfr->cache_change) && (!target->changes_for_reader->insertValue(cfr)))
					{
						printf("\t(FALSE)size==%d\n", target->changes_for_reader->getValidSize());
						delete(cfr);
						break;
					}
					else
					{
						printf("\t(TRUE)size==%d\n", target->changes_for_reader->getValidSize());
						if(target->changes_for_reader->getValidSize() == 3)
						{
							printf("HERE!!!!\n");
						}
						result	= true;
					}
				}
			}*/
		}

		if(result)
		{
			event			= new EntityEvent();
			event->data		= 0;
			event->target	= (unsigned long)proxy;

			if(this->push_mode == true)
			{
				event->event	= UNSENT_CHANGES_TRUE;
			}
			else
			{
				event->event	= UNACKED_CHANGES_TRUE;
			}
			this->SetEvent(event);
		}

		return result;
	}

	void			StatefulWriter::matched_reader_add(ReaderProxy* a_reader_proxy)
	{
		matched_readers->insertInRear(a_reader_proxy);
		a_reader_proxy->start();
	}

	void			StatefulWriter::matched_reader_remove(ReaderProxy* a_reader_proxy)
	{
		Node<ReaderProxy>*	head		= matched_readers->getNodeByIndex(0);
		Node<ReaderProxy>*	cur_node	= head->rear;

		matched_readers->listLock();
		while(cur_node != head)
		{
			if(cur_node->value == a_reader_proxy)
			{
				matched_readers->listUnlock();
				matched_readers->cutNode(cur_node);
				matched_readers->listLock();
				delete(a_reader_proxy);
				break;
			}

			cur_node	= cur_node->rear;
		}
		matched_readers->listUnlock();
	}

	ReaderProxy*	StatefulWriter::matched_reader_lookup(GUID_t a_reader_guid)
	{
		ReaderProxy*		result		= NULL;
		Node<ReaderProxy>*	head		= matched_readers->getNodeByIndex(0);
		Node<ReaderProxy>*	cur_node	= head->rear;

		matched_readers->listLock();
		while(cur_node != head)
		{
			if(cur_node->value->remote_reader_guid == a_reader_guid)
			{
				result	= cur_node->value;
			}

			cur_node	= cur_node->rear;
		}
		matched_readers->listUnlock();

		return	result;
	}

	bool			StatefulWriter::is_acked_by_all(CacheChange* a_change)
	{
		Node<ReaderProxy>*	head		= matched_readers->getNodeByIndex(0);
		Node<ReaderProxy>*	cur_node	= head->rear;

		matched_readers->listLock();
		while(cur_node != head)
		{
			if(cur_node->value->is_acked(a_change->sequence_number) == false)
			{
				matched_readers->listUnlock();
				return false;
			}

			cur_node	= cur_node->rear;
		}
		matched_readers->listUnlock();

		return	true;
	}

	CacheChange*		StatefulWriter::new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle)
	{
		CacheChange*		result;
		ChangeForReader*	cfr;
		int					i;
		ReaderProxy*		proxy;
		EntityEvent*		event;

		last_change_sequence_number	+= SequenceNumber_t(0,1);

		result					= new CacheChange();
		result->kind			= kind;
		memcpy(&result->writer_guid, &this->guid, sizeof(GUID_t));
		result->sequence_number	= last_change_sequence_number;
		result->data_value		= data;
		result->instance_handle	= handle;

		writer_cache->add_change(result);

		//printf("StatefulWriter::matched_readers(%d)\n", matched_readers->getSize());
		for(i=1; i<=matched_readers->getSize(); i++)
		{
			
			cfr		= new ChangeForReader();
#ifdef SHARED_PTR_CACHE_CHANGE
			cfr->cache_change	= SHARED_PTR_TYPE_NATIVE<CacheChange>(result);
#else
			cfr->cache_change	= result;
#endif
			cfr->is_relevant	= true;

			if(this->push_mode == true)
			{
				cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
			}
			else
			{
				cfr->status			= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
			}

			proxy	= matched_readers->getNodeByIndex(i)->value;
			proxy->changes_for_reader->insertValue(cfr);

			event			= new EntityEvent();
			event->data		= 0;
			event->target	= (unsigned long)proxy;

			if(this->push_mode == true)
			{
				event->event	= UNSENT_CHANGES_TRUE;
			}
			else
			{
				event->event	= UNACKED_CHANGES_TRUE;
			}
			this->SetEvent(event);
			

			//matched_readers->getNodeByIndex(i)->value->refillChangesForReader();
		}

		return	result;
	}

	void*	StatefulWriter::_get_transceiver()
	{
		TransceiverManager*	transceiver_manager	= TransceiverManager::getManagerInstance();
		Transceiver*		transceiver;
		unsigned long		transceiver_id;
		unsigned long		domain_id	= this->related_dcps_writer->related_participant->get_domain_id();

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

	void*	StatefulWriter::processing(void* arg)
	{
		StatefulWriter*	writer	= (StatefulWriter*)arg;
		EntityEvent*	event;
		ReaderProxy*	reader_proxy;
		Transceiver*	transceiver;
		int				i;
		GUID_t			reader_guid;
		Message*		message;
		AckNack*		submessage_acknack;
		bool			is_useful;

		transceiver	= (Transceiver*)writer->_get_transceiver();

		while(writer->is_running)
		{
			while(writer->events->getSize() > 0)
			{
				event	= writer->events->popFirst();

				switch((WriterEvent)event->event)
				{
				case SEND_A_MESSAGE:
					if(transceiver)
					{
						reader_proxy	= (ReaderProxy*)event->target;
						transceiver->send((ByteStream*)event->data, *reader_proxy->unicast_locator_list->getFirst());
						delete(event);
					}
					else
					{
						transceiver	= (Transceiver*)writer->_get_transceiver();
						writer->SetEvent(event);
					}
					break;

				case ACKNACK_IS_RECEIVED:
//printf("ACKNACK_IS_RECEIVED\n");
					message				= (Message*)event->data;
					submessage_acknack	= (AckNack*)message->submessages->getFirst();

					reader_guid.guid_prefix	= message->header.guid_prefix;
					reader_guid.entity_id	= submessage_acknack->reader_id.value;

					is_useful				= false;

//printf("StatefulWriter::matched_readers(%d)\n", writer->matched_readers->getSize());
					for(i=1; i<=writer->matched_readers->getSize(); i++)
					{
						reader_proxy		= writer->matched_readers->getNodeByIndex(i)->value;

//printf("\t proxy   %08X %08X %08X\n", *((unsigned long*)reader_proxy->remote_reader_guid.guid_prefix.host_id), *((unsigned long*)reader_proxy->remote_reader_guid.guid_prefix.app_id), *((unsigned long*)reader_proxy->remote_reader_guid.guid_prefix.counter));
//printf("\t message %08X %08X %08X\n", *((unsigned long*)reader_guid.guid_prefix.host_id), *((unsigned long*)reader_guid.guid_prefix.app_id), *((unsigned long*)reader_guid.guid_prefix.counter));
//printf("\t proxy   %s\n", reader_proxy->remote_reader_guid.entity_id==ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER?"PUBLICATION_READER":"SUBSCRIPTION_READER");
//printf("\t message %s\n", reader_guid.entity_id==ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER?"PUBLICATION_READER":"SUBSCRIPTION_READER");

						if(reader_proxy->remote_reader_guid == reader_guid)
						{
//printf("\t\tENTER\n");
							reader_proxy->set_event((WriterEvent)event->event, event->data);
							is_useful	= true;
							break;
						}
						else
						{
//printf("\t\tCANNOT FOUND MATCHED READER PROXY\n");
						}
					}

					if(is_useful == false)
					{
						delete(message);
						delete(event);
					}
					break;

				default:
//printf("DEFAULT\n");
					reader_proxy	= (ReaderProxy*)event->target;
					reader_proxy->set_event((WriterEvent)event->event, event->data);
					delete(event);
					break;
				}
			}
//printf("SLEEP(%08X)\n", writer);
			writer->Wait();
//printf("WAKE UP(%08X)\n", writer);
		}

		return 0;
	}
}
