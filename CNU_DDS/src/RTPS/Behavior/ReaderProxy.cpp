#include "../../../include/RTPS/Behavior/ReaderProxy.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"
#include "../../../include/Utility/Timer/DynamicTimer.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"
#include "../../../include/Utility/SystemInformation.h"

#include <stdio.h>

namespace CNU_DDS
{
	ReaderProxy::ReaderProxy()
	{
		heartbeat_count			= 1;
		//tmp_count	= 0;
		//printf("ReaderProxy::ReaderProxy()\n");
		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_for_reader		= NULL;
		expects_inline_qos		= false;
		is_running				= false;
		wait_a_heartbeating		= false;
	}

	ReaderProxy::ReaderProxy(unsigned long writer_instance)
	{
		heartbeat_count			= 1;
		//tmp_count	= 0;
		//printf("ReaderProxy::ReaderProxy()\n");
		EntityEvent*		entity_event	= NULL;

		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_for_reader		= new ChangeForReaderList(READER_PROXY_CACHE_SIZE_MAX);
		related_writer			= writer_instance;
		expects_inline_qos		= false;
		is_running				= false;
		wait_a_heartbeating		= false;

		if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			pushing_state		= WRITER_STATE_IDLE;
			repairing_state		= WRITER_STATE_IDLE;
		}
		else if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_RELIABLE)
		{
			pushing_state		= WRITER_STATE_ANNOUNCING;
			repairing_state		= WRITER_STATE_WAITING;
		}
		else
		{
			//error
		}
	}

	ReaderProxy::ReaderProxy(unsigned long writer_instance, unsigned long max_cache_size)
	{
		heartbeat_count			= 1;
		//tmp_count	= 0;
		//printf("ReaderProxy::ReaderProxy()\n");
		EntityEvent*		entity_event	= NULL;

		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_for_reader		= new ChangeForReaderList(max_cache_size);
		related_writer			= writer_instance;
		expects_inline_qos		= false;
		is_running				= false;
		wait_a_heartbeating		= false;

		if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			pushing_state		= WRITER_STATE_IDLE;
			repairing_state		= WRITER_STATE_IDLE;
		}
		else if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_RELIABLE)
		{
			pushing_state		= WRITER_STATE_ANNOUNCING;
			repairing_state		= WRITER_STATE_WAITING;
		}
		else
		{
			//error
		}
	}

	ReaderProxy::~ReaderProxy()
	{
		//printf("ReaderProxy::~ReaderProxy()\n");
		delete(unicast_locator_list);
		delete(multicast_locator_list);

		if(changes_for_reader)
		{
			delete(changes_for_reader);
		}
	}

	void				ReaderProxy::acked_changes_set(SequenceNumber_t committed_seq_num)
	{
		//printf("ReaderProxy::acked_changes_set()\n");
		SequenceNumber_t	calc_number;
		bool				result;

		if( changes_for_reader->getBaseNumber() <= committed_seq_num )
		{
			calc_number	=	( committed_seq_num - changes_for_reader->getBaseNumber() );
		}
		else
		{
			return;
		}

		for(int i=0; i<=calc_number.low; i++)
		{
			result	= changes_for_reader->changeStatusByIndex(i, CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED);
		}
	}

	ChangeForReader*	ReaderProxy::next_requested_change(void)
	{
		//printf("ReaderProxy::next_requested_change()\n");
		return changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_REQUESTED);
	}

	ChangeForReader*	ReaderProxy::next_unsent_change(void)
	{
		//printf("ReaderProxy::next_unsent_change()\n");
		return changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT);
	}
/*
	ChangeForReaderSeq*	ReaderProxy::unsent_changes(void)
	{
		return NULL;
	}

	ChangeForReaderSeq*	ReaderProxy::requested_changes(void)
	{
		return NULL;
	}

	ChangeForReaderSeq*	ReaderProxy::unacked_changes(void)
	{
		return NULL;
	}
*/
	bool				ReaderProxy::is_acked(SequenceNumber_t seq_num)
	{
		//printf("ReaderProxy::is_acked()\n");
		ChangeForReader*	cur_node	= changes_for_reader->getValueBySequenceNumber(seq_num);

		if( (cur_node != NULL) && (cur_node->status == CHANGE_FOR_READER_STATUS_KIND_ACKNOWLEDGED) )
		{
			return true;
		}
		else if( cur_node == NULL )
		{
			return true;
		}

		return false;
	}

	void				ReaderProxy::requested_changes_set(SequenceNumberSeq* req_seq_num_set)
	{
		//printf("ReaderProxy::requested_changes_set()\n");
		SequenceNumber_t*	cur_seq	= NULL;
		ChangeForReader*	cfr		= NULL;

		//printf("\treq_seq_num_set->getSize() == %d\n", req_seq_num_set->getSize());
		while(req_seq_num_set->getSize() > 0)
		{
			cur_seq	= req_seq_num_set->popFirst();

			cfr	= changes_for_reader->getValueBySequenceNumber(*cur_seq);
			//printf("\tcur_seq(%08X, %d, %d, %d)\n", cfr, cur_seq->high, cur_seq->low, cfr!=NULL?cfr->status:-1);

			if((cfr != NULL) && (cfr->status == CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED))
			{
				//printf("\tchangeStatusBySequenceNumber\n");
				changes_for_reader->changeStatusBySequenceNumber(*cur_seq, CHANGE_FOR_READER_STATUS_KIND_REQUESTED);
			}

			delete(cur_seq);
		}
	}

	void			ReaderProxy::start()
	{
		//printf("ReaderProxy::start()\n");
		StatefulWriter*		writer	= ((StatefulWriter*)related_writer);
		int					i;
		ChangeForReader*	cfr;
		bool				has_changes	= false;
		EntityEvent*		event;

		if(is_running == false)
		{
			is_running = true;

			for(i=1; i<=writer->writer_cache->changes->getSize(); i++)
			{
				has_changes	= true;

				cfr	= new ChangeForReader();
				cfr->cache_change	= writer->writer_cache->changes->getNodeByIndex(i)->value;
				cfr->is_relevant	= true;

				if(writer->push_mode)
				{
					cfr->status	= CHANGE_FOR_READER_STATUS_KIND_UNSENT;
				}
				else
				{
					cfr->status	= CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED;
				}

				changes_for_reader->insertValue(cfr);
			}

			if(writer->push_mode && has_changes)
			{
				event			= new EntityEvent();
				event->data		= 0;
				event->target	= (unsigned long)this;
				event->event	= UNSENT_CHANGES_TRUE;
				writer->SetEvent(event);
			}
			else if(writer->push_mode == false && writer->reliability_level == RELIABILITY_KIND_RELIABLE)
			{
				_send_a_heartbeat_message();
			}
			else if(has_changes == false && writer->push_mode == true && writer->reliability_level == RELIABILITY_KIND_RELIABLE)
			{
				_send_a_heartbeat_message();
			}
		}
	}

	void*			ReaderProxy::_create_a_data_message(EntityId_t* reader_id, CacheChange* change)
	{
		//printf("ReaderProxy::_create_a_data_message()\n");
		Message*			message			= NULL;
		Data*				submessage_data	= NULL;
		StatefulWriter*		writer			= (StatefulWriter*)related_writer;

		message					= new Message();
		message->header.guid_prefix		= writer->guid.guid_prefix;
		message->header.protocol		= PROTOCOL_RTPS;
		message->header.version.major	= PROTOCOL_VERSION.major;
		message->header.version.minor	= PROTOCOL_VERSION.minor;

		submessage_data			= new Data();
		submessage_data->submessage_header.submessage_id	= SUBMESSAGE_KIND_DATA;
		submessage_data->submessage_header.flags			= 0x04;
		submessage_data->writer_id.value					= writer->guid.entity_id;
		submessage_data->reader_id.value					= *reader_id;

		submessage_data->writer_sn.value			= change->sequence_number;
		submessage_data->serialized_payload.value	= *change->data_value;
		switch(change->data_value->getByteOrderingType())
		{
		case BYTE_ORDERING_TYPE_BIG_ENDIAN:
			memcpy(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_BE, 4);
			break;
		case BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST:
			memcpy(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_PL_CDR_BE, 4);
			break;
		case BYTE_ORDERING_TYPE_LITTLE_ENDIAN:
			memcpy(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_LE, 4);
			submessage_data->submessage_header.flags	+= 0x01;
			break;
		case BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST:
			memcpy(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_PL_CDR_LE, 4);
			submessage_data->submessage_header.flags	+= 0x01;
			break;
		}

		message->submessages->insertInRear(submessage_data);

		return message;
	}

	void*				ReaderProxy::_create_a_heartbeat_message(EntityId_t* reader_id, char flags)
	{
		//printf("ReaderProxy::_create_a_heartbeat_message()\n");
		Message*			message			= NULL;
		Heartbeat*			submessage		= NULL;
		StatefulWriter*		writer			= (StatefulWriter*)related_writer;
		ByteOrderingType	ordering		= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();
		SequenceNumber		sn;

		if(ordering == BYTE_ORDERING_TYPE_LITTLE_ENDIAN)
		{
			flags	|= 0x01;
		}

		message							= new Message();
		message->header.guid_prefix		= writer->guid.guid_prefix;
		message->header.protocol		= PROTOCOL_RTPS;
		message->header.version.major	= PROTOCOL_VERSION.major;
		message->header.version.minor	= PROTOCOL_VERSION.minor;

		submessage									= new Heartbeat();
		submessage->submessage_header.flags			= flags;
		submessage->submessage_header.submessage_id	= SUBMESSAGE_KIND_HEARTBEAT;
		submessage->writer_id.value					= writer->guid.entity_id;
		submessage->reader_id.value					= *reader_id;
		//if(writer->writer_cache->changes->getSize())
		if(changes_for_reader->getValidSize())
		{
			//submessage->first_sn.value					= writer->writer_cache->get_seq_num_min();
			//submessage->last_sn.value					= writer->writer_cache->get_seq_num_max();
			submessage->first_sn.value	= changes_for_reader->getBaseNumber();
			submessage->last_sn.value	= changes_for_reader->getValueByIndex(changes_for_reader->getValidSize()-1)->cache_change->sequence_number;
		}
		else
		{
			/*before*/
			/*
			submessage->first_sn.value.high	= 0;
			submessage->first_sn.value.low	= 0;
			submessage->last_sn.value.high	= 0;
			submessage->last_sn.value.low	= 1;
			*/

			/*after*/
			submessage->first_sn.value.high	= 0;
			submessage->first_sn.value.low	= 1;
			submessage->last_sn.value.high	= 0;
			submessage->last_sn.value.low	= 0;
		}
		submessage->count.value.value				= heartbeat_count++;

		message->submessages->insertInRear(submessage);

		return message;
	}

	void				ReaderProxy::_after_heartbeat_period(unsigned long arg)
	{
		//printf("ReaderProxy::_after_heartbeat_period()\n");
		ReaderProxy*	proxy	= (ReaderProxy*)arg;
		EntityEvent*	event	= new EntityEvent();

		event->target	= arg;
		event->event	= AFTER_HEARTBEAT_PERIOD;
		event->data		= 0;

		proxy->wait_a_heartbeating	= false;

		((StatefulWriter*)proxy->related_writer)->SetEvent(event);
	}

	void				ReaderProxy::_after_nack_response_delay(unsigned long arg)
	{
		//printf("ReaderProxy::_after_nack_response_delay()\n");
		ReaderProxy*	proxy	= (ReaderProxy*)arg;
		EntityEvent*	event	= new EntityEvent();

		event->target	= arg;
		event->event	= AFTER_NACK_RESPONSE_DELAY;
		event->data		= 0;

		((StatefulWriter*)proxy->related_writer)->SetEvent(event);
	}

	void			ReaderProxy::_after_nack_suppression_duration(unsigned long arg)
	{
		//printf("ReaderProxy::_after_nack_suppression_duration()\n");
		ReaderProxy*		proxy		= (ReaderProxy*)arg;
		EntityEvent*		event		= NULL;
		ChangeForReader*	cfr		= NULL;
		bool				has_unacked	= false;

		cfr	= proxy->changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
		while(cfr != NULL)
		{
			has_unacked	= true;

			proxy->changes_for_reader->changeStatusBySequenceNumber(cfr->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);

			cfr	= proxy->changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
		}

		if(has_unacked)
		{
			event			= new EntityEvent();
			event->data		= 0;
			event->target	= arg;
			event->event	= UNACKED_CHANGES_TRUE;
			((StatefulWriter*)proxy->related_writer)->SetEvent(event);
		}
	}

	void				ReaderProxy::_send_a_heartbeat_message()
	{
		//printf("ReaderProxy::_send_a_heartbeat_message()\n");
		EntityId_t		reader_id				= ENTITYID_UNKNOWN;
		Message*		message					= (Message*)_create_a_heartbeat_message(&remote_reader_guid.entity_id, 0x00);
		EntityEvent*	entity_event;

		entity_event			= new EntityEvent();
		entity_event->target	= (unsigned long)this;
		entity_event->event		= SEND_A_MESSAGE;
		entity_event->data		= (unsigned long)message->Serialize();

		((Writer*)related_writer)->SetEvent(entity_event);

		message->submessages->deleteAll();
		delete(message);

		// register TIMER for HEARTBEAT
		if(!wait_a_heartbeating)
		{
			wait_a_heartbeating		= true;

			DynamicTimer*	timer	= DynamicTimer::getDynamicTimerInstance();

			TimeUnit*	time_unit	= new TimeUnit(((Writer*)related_writer)->heartbeat_period.getDurationByMilliSecond());
			time_unit->setDataValue((unsigned long)this);
			time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

			timer->regist(time_unit);
		}
	}

	bool				ReaderProxy::refillChangesForReader()
	{
		//printf("ReaderProxy(%08X)::refillChangesForReader()\n", this);
		bool				result	= false;
		SequenceNumber_t	seq;

		if( changes_for_reader->refreshByStatus() || (changes_for_reader->getValidSize() < changes_for_reader->getLimitSize()) )
		{
			if(changes_for_reader->getValidSize() > 0)
			{
				//printf("[%d]Value(%08X)\n", changes_for_reader->getValidSize()-1, changes_for_reader->getValueByIndex(changes_for_reader->getValidSize()-1));
				seq	= changes_for_reader->getValueByIndex(changes_for_reader->getValidSize()-1)->cache_change->sequence_number + SequenceNumber_t(0, 1);

				if(((StatefulWriter*)related_writer)->_insertCacheChangesIntoReaderProxy((unsigned long)this, seq))
				{
					result	= true;
				}
			}
			else if(((StatefulWriter*)related_writer)->_insertCacheChangesIntoReaderProxy((unsigned long)this, changes_for_reader->getBaseNumber()))
			{
				result	= true;
			}
		}

		return	result;
	}

	void				ReaderProxy::set_event(WriterEvent event, unsigned long data)
	{
		//printf("ReaderProxy(%08X)::set_event(ps:%d, rs:%d, e:%d)\n",this, pushing_state, repairing_state, event);
		/*if(event == UNSENT_CHANGES_TRUE)
		{
			tmp_count++;
=		printf("ReaderProxy(%08X) UCT(s:%d, count:%d)\n", this, pushing_state, tmp_count);
		}*/
		/*
		if((pushing_state == WRITER_STATE_PUSHING) && !((event == UNSENT_CHANGES_TRUE) || (event == CAN_PUSHING)))
		{
			printf("ReaderProxy(%08X)::set_event(ps:%d, rs:%d, e:%d)\n",this, pushing_state, repairing_state, event);
		}*/

		StatefulWriter*		writer			= (StatefulWriter*)related_writer;
		ChangeForReader*	change			= NULL;
		EntityEvent*		entity_event	= NULL;

		Message*			message				= NULL;
		AckNack*			submessage_acknack	= NULL;
		EntityId_t			reader_id;

		DynamicTimer*		timer			= NULL;
		TimeUnit*			time_unit		= NULL;

		if(writer->reliability_level == RELIABILITY_KIND_BEST_EFFORT) /* BEST_EFFORT *** BEST_EFFORT *** BEST_EFFORT *** BEST_EFFORT  *** BEST_EFFORT  *** BEST_EFFORT */
		{
			switch(event)
			{
			case UNSENT_CHANGES_TRUE:	// Best-effort T2

				if( pushing_state == WRITER_STATE_IDLE )
				{
					//printf("ReaderProxy(%08X) Best-effort T2\n", this);

					pushing_state	= WRITER_STATE_PUSHING;

					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= CAN_PUSHING;
					entity_event->data		= 0;

					writer->SetEvent(entity_event);
				}

				break;

			case UNSENT_CHANGES_FALSE:	// Best-effort T3

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					//printf("ReaderProxy(%08X) Best-effort T3\n", this);

					pushing_state	= WRITER_STATE_IDLE;
				}

				break;

			case CAN_PUSHING:	// Best-effort T4

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					//printf("ReaderProxy(%08X) Best-effort T4\n", this);

					change	= next_unsent_change();
					changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);

					if( (change != NULL) && (change->is_relevant) )
					{
						/*
						 DATA	= new DATA(change);
						 IF (the_reader_proxy.expectsInlineQos)
						 {
						 	 DATA.inlineQos	= the_rtps_writer.related_dds_writer.qos;
						 }
						 DATA.readerId	= ENTITYID_UNKNOWN;
						 send DATA
						 */
						reader_id				= remote_reader_guid.entity_id;
#ifdef SHARED_PTR_CACHE_CHANGE
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change.get());
#else
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change);
#endif

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= SEND_A_MESSAGE;
						entity_event->data		= (unsigned long)message->Serialize();

						writer->SetEvent(entity_event);

						message->submessages->deleteAll();
						delete(message);

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= CAN_PUSHING;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else if ( change == NULL)
					{
						//printf("ReaderProxy(%08X) Best-effort T4->T3\n", this);

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
				}

				break;

			case DELETE_READER_PROXY:	// Best-effort T6
				break;

			default:
				break;
			}
		}
		else if(writer->reliability_level == RELIABILITY_KIND_RELIABLE) /* RELIABLE *** RELIABLE *** RELIABLE *** RELIABLE  *** RELIABLE  *** RELIABLE *** RELIABLE *** RELIABLE */
		{
			switch(event)
			{
			case UNSENT_CHANGES_TRUE:	// Reliable T2

				//printf("\tUNSENT_CHANGES_TRUE\n");
				if( (pushing_state == WRITER_STATE_ANNOUNCING) || (pushing_state == WRITER_STATE_IDLE) )
				{
					//printf("ReaderProxy(%08X) Reliable T2\n", this);

					pushing_state	= WRITER_STATE_PUSHING;

					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= CAN_PUSHING;
					entity_event->data		= 0;

					writer->SetEvent(entity_event);
				}

				break;

			case UNSENT_CHANGES_FALSE:	// Reliable T3

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					//printf("ReaderProxy(%08X) Reliable T3\n", this);

					pushing_state	= WRITER_STATE_ANNOUNCING;

					if(changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT) != NULL)
					{
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_TRUE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else
					{
						// register TIMER for AckNack suppression delay
						if(writer->nack_suppression_duration.getDurationByMilliSecond())
						{
							timer	= DynamicTimer::getDynamicTimerInstance();

							time_unit	= new TimeUnit(writer->nack_suppression_duration.getDurationByMilliSecond());
							time_unit->setDataValue((unsigned long)this);
							time_unit->setExecuteFunction(ReaderProxy::_after_nack_suppression_duration);

							timer->regist(time_unit);
						}

						// register TIMER for HEARTBEAT
						if(!wait_a_heartbeating)
						{
							wait_a_heartbeating		= true;

							timer	= DynamicTimer::getDynamicTimerInstance();

							time_unit	= new TimeUnit(writer->heartbeat_period.getDurationByMilliSecond());
							time_unit->setDataValue((unsigned long)this);
							time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

							timer->regist(time_unit);
						}
					}
				}

				break;

			case UNACKED_CHANGES_TRUE:	// Reliable T6

				if( pushing_state == WRITER_STATE_IDLE )
				{
					//printf("ReaderProxy(%08X) Reliable T6\n", this);

					pushing_state	= WRITER_STATE_ANNOUNCING;

					// register TIMER for HEARTBEAT
					if(!wait_a_heartbeating)
					{
						wait_a_heartbeating		= true;

						timer	= DynamicTimer::getDynamicTimerInstance();

						time_unit	= new TimeUnit(writer->heartbeat_period.getDurationByMilliSecond());
						time_unit->setDataValue((unsigned long)this);
						time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

						timer->regist(time_unit);
					}
				}

				break;

			case UNACKED_CHANGES_FALSE:	// Reliable T5

				if( pushing_state == WRITER_STATE_ANNOUNCING )
				{
					//printf("ReaderProxy(%08X) Reliable T5\n", this);

					/*refreshByStatus()*/
					
					if(refillChangesForReader())
					{
						// refillChangesForReader()->_insertCacheChangesIntoReaderProxy() generates a Event //Do nothing
						//printf("refill is over\n");
						//printf("wait_a_heartbeating(%d)\n", wait_a_heartbeating);

						if(writer->push_mode == true)
						{
							entity_event			= new EntityEvent();
							entity_event->target	= (unsigned long)this;
							entity_event->event		= UNSENT_CHANGES_TRUE;
							entity_event->data		= 0;

							writer->SetEvent(entity_event);
						}
						else
						{
							// register TIMER for HEARTBEAT
							if(!wait_a_heartbeating)
							{
								wait_a_heartbeating		= true;

								timer	= DynamicTimer::getDynamicTimerInstance();

								time_unit	= new TimeUnit(writer->heartbeat_period.getDurationByMilliSecond());
								time_unit->setDataValue((unsigned long)this);
								time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

								timer->regist(time_unit);
							}
						}
					}
					else if( repairing_state == WRITER_STATE_WAITING )
					{
						//printf("This ReaderProxy(%08X) doesn't have data for sending\n", this);
						pushing_state	= WRITER_STATE_IDLE;
					}
					
					/*
					if( repairing_state == WRITER_STATE_WAITING )
					{
						pushing_state	= WRITER_STATE_IDLE;
					}*/
				}

				break;

			case REQUESTED_CHANGES_TRUE:	// Reliable T9

				if( repairing_state == WRITER_STATE_WAITING )
				{
					//printf("ReaderProxy(%08X) Reliable T9\n", this);

					repairing_state	= WRITER_STATE_MUST_REPAIR;

					// register TIMER for NackResponseDelay
					timer	= DynamicTimer::getDynamicTimerInstance();

					time_unit	= new TimeUnit(writer->nack_response_delay.getDurationByMilliSecond());
					time_unit->setDataValue((unsigned long)this);
					time_unit->setExecuteFunction(ReaderProxy::_after_nack_response_delay);

					timer->regist(time_unit);
				}

				break;

			case REQUESTED_CHANGES_FALSE:	// Reliable T13

				if( repairing_state == WRITER_STATE_REPAIRING)
				{
					//printf("ReaderProxy(%08X) Reliable T13\n", this);

					repairing_state	= WRITER_STATE_WAITING;

					// register TIMER for AckNack suppression delay
					if(writer->nack_suppression_duration.getDurationByMilliSecond())
					{
						timer	= DynamicTimer::getDynamicTimerInstance();

						time_unit	= new TimeUnit(writer->nack_suppression_duration.getDurationByMilliSecond());
						time_unit->setDataValue((unsigned long)this);
						time_unit->setExecuteFunction(ReaderProxy::_after_nack_suppression_duration);

						timer->regist(time_unit);
					}

					// register TIMER for HEARTBEAT
					if(!wait_a_heartbeating)
					{
						wait_a_heartbeating		= true;

						timer	= DynamicTimer::getDynamicTimerInstance();

						time_unit	= new TimeUnit(writer->heartbeat_period.getDurationByMilliSecond());
						time_unit->setDataValue((unsigned long)this);
						time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

						timer->regist(time_unit);
					}
				}

				break;

			case AFTER_HEARTBEAT_PERIOD:	// Reliable T7

				if( pushing_state == WRITER_STATE_ANNOUNCING )
				{
					//printf("ReaderProxy(%08X) Reliable T7\n", this);
////printf("\tUNACKED:  %08X\n", changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED));
////printf("\tUNDERWAY: %08X\n", changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY));

					change	= changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT);
					//if(changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNSENT) != NULL)
					if(change != NULL)
					{
//printf("UNSENT: %d\n", change->cache_change->sequence_number.low);
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_TRUE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else if((changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED)==NULL) && (changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY)==NULL))
					{
//printf("\t(T7) UNACKED_CHANGES_FALSE\n");
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNACKED_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else
					{
/*
printf("\t(T7) UNACKED_CHANGES_TRUE\n");
change	= changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
if(change && (change->status != CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED))
{
	printf("UNACKNOWLEDGED: %d (state:%d)\n", change->cache_change->sequence_number.low, change->status);
}
change	= changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
if(change)
{
	printf("UNDERWAY: %d\n", change->cache_change->sequence_number.low);
}*/
						// send HEARTBEAT(FinalFlag = NOT_SET)

						//reader_id				= ENTITYID_UNKNOWN;
						reader_id				= remote_reader_guid.entity_id;
						message					= (Message*)_create_a_heartbeat_message(&reader_id, 0x00);

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= SEND_A_MESSAGE;
						entity_event->data		= (unsigned long)message->Serialize();

						writer->SetEvent(entity_event);

						message->submessages->deleteAll();
						delete(message);

						// register TIMER for HEARTBEAT
						if(!wait_a_heartbeating)
						{
							wait_a_heartbeating		= true;

							timer	= DynamicTimer::getDynamicTimerInstance();

							time_unit	= new TimeUnit(writer->heartbeat_period.getDurationByMilliSecond());
							time_unit->setDataValue((unsigned long)this);
							time_unit->setExecuteFunction(ReaderProxy::_after_heartbeat_period);

							timer->regist(time_unit);
						}
					}
				}

				break;

			case AFTER_NACK_RESPONSE_DELAY:	// Reliable T11

				if( repairing_state == WRITER_STATE_MUST_REPAIR )
				{
					//printf("ReaderProxy(%08X) Reliable T11\n", this);

					repairing_state	= WRITER_STATE_REPAIRING;

					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= CAN_REPAIRING;
					entity_event->data		= 0;

					writer->SetEvent(entity_event);
				}

				break;

			case CAN_PUSHING:	// Reliable T4

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					//printf("ReaderProxy(%08X) Reliable T4\n", this);

					change	= next_unsent_change();
////printf("\tchange(%08X)\n", change);
					if( (change != NULL) && (change->is_relevant) )
					{
						if(writer->nack_suppression_duration.getDurationByMilliSecond())
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
						}
						else
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
						}

						/*
						 DATA	= new DATA(change);
						 IF ( the_reader_proxy.expectsInlineQos )
						 {
						 	 DATA.inlineQos	= the_rtps_writer.related_dds_writer.qos;
						 }
						 DATA.readerId	= ENTITYID_UNKNOWN;
						 send DATA
						 */
						//reader_id				= ENTITYID_UNKNOWN;
						reader_id				= this->remote_reader_guid.entity_id;
#ifdef SHARED_PTR_CACHE_CHANGE
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change.get());
#else
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change);
#endif

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= SEND_A_MESSAGE;
						entity_event->data		= (unsigned long)message->Serialize();

						writer->SetEvent(entity_event);

////printf("\tSEND A DATA!!\n");
						message->submessages->deleteAll();
						delete(message);

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= CAN_PUSHING;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else if( change != NULL )
					{
						if(writer->nack_suppression_duration.getDurationByMilliSecond())
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
						}
						else
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
						}

						/*
						 GAP	= new GAP(change->sequenceNumber);
						 GAP.readerId	= ENTITYID_UNKNOWN;
						 send GAP;
						 */
/*
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= CAN_PUSHING;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
*/
					}
					else
					{
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
				}

				break;

			case CAN_REPAIRING:	// Reliable T12

				if( repairing_state == WRITER_STATE_REPAIRING )
				{
					//printf("ReaderProxy(%08X) Reliable T12\n", this);

					change	= next_requested_change();

					if( (change != NULL) && (change->is_relevant) )
					{
						/*
						 DATA	= new DATA(change, the_reader_proxy.remoteReaderGuid);
						 IF ( the_reader_proxy.expectsInlineQos )
						 {
						 	 DATA.inlineQos	= the_rtps_writer.related_dds_writer.qos;
						 }
						 send DATA;
						 */

						if(writer->nack_suppression_duration.getDurationByMilliSecond())
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNDERWAY);
						}
						else
						{
							changes_for_reader->changeStatusBySequenceNumber(change->cache_change->sequence_number, CHANGE_FOR_READER_STATUS_KIND_UNACKNOWLEDGED);
						}

						reader_id				= remote_reader_guid.entity_id;
#ifdef SHARED_PTR_CACHE_CHANGE
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change.get());
#else
						message					= (Message*)_create_a_data_message(&reader_id, change->cache_change);
#endif

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= SEND_A_MESSAGE;
						entity_event->data		= (unsigned long)message->Serialize();

						writer->SetEvent(entity_event);

						message->submessages->deleteAll();
						delete(message);

						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= CAN_REPAIRING;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else if( change != NULL )
					{
						/*
						 GAP	= new GAP(change.sequenceNumber, the_reader_proxy.remoteReaderGuid);
						 send GAP;
						 */
/*
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= CAN_REPAIRING;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
*/
					}
					else
					{
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= REQUESTED_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
				}

				break;

			case ACKNACK_IS_RECEIVED:	// Reliable T8, T10
//printf("\t(T8, T10) ReaderProxy ACKNACK_IS_RECEIVED\n");

				if( repairing_state == WRITER_STATE_WAITING )
				{
//printf("ReaderProxy(%08X) Reliable T8\n", this);
					//printf("\tNackResponseDelay(%d)\n", writer->nack_response_delay.getDurationByMilliSecond());

					/*
					 acked_changes_set(ACKNACK.readerSNState.base - 1);
					 the_reader_proxy.requested_changes_set(ACKNACK.readerSNState.set);
					 */

					message	= (Message*)data;
					submessage_acknack	= (AckNack*)message->submessages->getFirst();

					acked_changes_set(submessage_acknack->reader_sn_state.base - SequenceNumber_t(0, 1));
					this->requested_changes_set(&submessage_acknack->reader_sn_state.set);

					message->submessages->deleteAll();
					delete(message);

					if(changes_for_reader->getFirstValueByStatus(CHANGE_FOR_READER_STATUS_KIND_REQUESTED) != NULL)
					{
						//printf("\tCHANGE_FOR_READER_STATUS_KIND_REQUESTED != NULL\n");
						repairing_state	= WRITER_STATE_MUST_REPAIR;

						timer	= DynamicTimer::getDynamicTimerInstance();

						time_unit	= new TimeUnit(writer->nack_response_delay.getDurationByMilliSecond());
						time_unit->setDataValue((unsigned long)this);
						time_unit->setExecuteFunction(ReaderProxy::_after_nack_response_delay);

						timer->regist(time_unit);
					}
				}
				else if( repairing_state == WRITER_STATE_MUST_REPAIR )
				{
//printf("ReaderProxy(%08X) Reliable T10\n", this);

					/*
					 acked_changes_set(ACKNACK.readerSNState.base - 1);
					 the_reader_proxy.requested_changes_set(ACKNACK.readerSNState.set);
					 */

					message	= (Message*)data;
					submessage_acknack	= (AckNack*)message->submessages->getFirst();

					acked_changes_set(submessage_acknack->reader_sn_state.base - SequenceNumber_t(0, 1));
					this->requested_changes_set(&submessage_acknack->reader_sn_state.set);

					message->submessages->deleteAll();
					delete(message);
				}

				break;

			case DELETE_READER_PROXY:	// Reliable T16
				break;

			default:
				break;
			}
		}
		else
		{
			//error
		}

		//printf("ReaderProxy(%08X)::clear(ps:%d, rs:%d)\n",this, pushing_state, repairing_state);
	}
}
