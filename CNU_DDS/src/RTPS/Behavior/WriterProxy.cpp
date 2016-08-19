#include "../../../include/RTPS/Behavior/WriterProxy.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"
#include "../../../include/RTPS/Behavior/ListenerProcessor.h"
#include "../../../include/RTPS/Structure/Reader.h"
#include "../../../include/Utility/Timer/DynamicTimer.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"
#include "../../../include/Utility/SystemInformation.h"
#include "../../../include/Utility/Network/Transceiver.h"

#include <stdio.h>

namespace CNU_DDS
{
	WriterProxy::WriterProxy()
	{
		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_from_writer		= NULL;
		is_running				= false;

		ack_count			= 1;
		//tmp_count			= 0;
		//tmp_create_count	= 0;
		//tmp_change_count	= 0;
	}

	WriterProxy::WriterProxy(unsigned long reader_instance)
	{
		//char	filename[16];
		//sprintf(filename, "%08X.csv", this);
		recent_available_seq	= SequenceNumber_t(0,1);
		//fp						= fopen(filename, "w+");
		//last_available_seq		= SequenceNumber_t(0,0);

		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_from_writer		= new ChangeFromWriterList(WRITER_PROXY_CACHE_SIZE_MAX);
		related_reader			= reader_instance;
		is_running				= false;

		if(((Reader*)reader_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			//printf("WriterProxy::BEST_EFFORT\n");
			waiting_state	= READER_STATE_WAITING;
			ready_state		= READER_STATE_UNKNOWN;
		}
		else if(((Reader*)reader_instance)->reliability_level == RELIABILITY_KIND_RELIABLE)
		{
			//printf("WriterProxy::RELIABLE\n");
			waiting_state	= READER_STATE_WAITING;
			ready_state		= READER_STATE_READY;
		}
		else
		{
			waiting_state	= READER_STATE_UNKNOWN;
			ready_state		= READER_STATE_UNKNOWN;
			//error
		}

		ack_count			= 1;
		//tmp_count			= 0;
		//tmp_create_count	= 0;
		//tmp_change_count	= 0;
	}

	WriterProxy::WriterProxy(unsigned long reader_instance, unsigned long max_cache_size)
	{
		//char	filename[16];
		//sprintf(filename, "%08X.csv", this);
		recent_available_seq	= SequenceNumber_t(0,1);
		//fp						= fopen(filename, "w+");
		//last_available_seq		= SequenceNumber_t(0,0);

		unicast_locator_list	= new LocatorSeq();
		multicast_locator_list	= new LocatorSeq();
		changes_from_writer		= new ChangeFromWriterList(max_cache_size);
		related_reader			= reader_instance;
		is_running				= false;

		if(((Reader*)reader_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			//printf("WriterProxy::BEST_EFFORT\n");
			waiting_state	= READER_STATE_WAITING;
			ready_state		= READER_STATE_UNKNOWN;
		}
		else if(((Reader*)reader_instance)->reliability_level == RELIABILITY_KIND_RELIABLE)
		{
			//printf("WriterProxy::RELIABLE\n");
			waiting_state	= READER_STATE_WAITING;
			ready_state		= READER_STATE_READY;
		}
		else
		{
			waiting_state	= READER_STATE_UNKNOWN;
			ready_state		= READER_STATE_UNKNOWN;
			//error
		}

		ack_count			= 1;
		//tmp_count			= 0;
		//tmp_create_count	= 0;
		//tmp_change_count	= 0;
	}

	WriterProxy::~WriterProxy()
	{
		delete(unicast_locator_list);
		delete(multicast_locator_list);
		if(changes_from_writer)
		{
			delete(changes_from_writer);
		}

		//fclose(fp);
	}

	SequenceNumber_t	WriterProxy::available_changes_max(void)
	{
		SequenceNumber_t	max_sn			= SequenceNumber_t(0,0);
		int					max_size		= changes_from_writer->getLimitSize();
		ChangeFromWriter*	cfw				= NULL;
		bool				is_founded		= false;

		if(changes_from_writer->getBaseNumber() <= recent_available_seq)
		{
			max_sn	= recent_available_seq - changes_from_writer->getBaseNumber();
		}

		int i	= max_sn.low;

		while(i<max_size)
		{
			cfw	= changes_from_writer->getValueByIndex(i);

			if( (cfw != NULL) && ( (cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_LOST) || (cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED) ) )
			{
				max_sn	= cfw->cache_change->sequence_number;
				is_founded	= true;
				i++;
			}
			else
			{
				break;
			}
		}

		if(is_founded)
		{
			recent_available_seq	= max_sn;
		}

		return max_sn;
		//return last_available_seq;
	}

	void				WriterProxy::irrelevant_change_set(SequenceNumber_t a_seq_num)
	{
		ChangeFromWriter*	cfw	= changes_from_writer->getValueBySequenceNumber(a_seq_num);

		if( cfw != NULL )
		{
			cfw->status			= CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED;
			cfw->is_relevant	= false;
		}
	}

	void				WriterProxy::lost_changes_update(SequenceNumber_t first_available_seq_num)
	{
		int 				i		= 0;
		ChangeFromWriter*	cfw		= NULL;

		for(i=0; i<changes_from_writer->getLimitSize(); i++)
		{
			cfw	= changes_from_writer->getValueByIndex(i);

			if( (cfw) && (cfw->cache_change->sequence_number < first_available_seq_num) )
			{
				changes_from_writer->changeStatusByIndex(i, CHANGE_FROM_WRITER_STATUS_KIND_LOST);
			}
			else if( (cfw) && (first_available_seq_num <= cfw->cache_change->sequence_number) )
			{
				break;
			}
		}
	}

	SequenceNumberSeq*	WriterProxy::missing_changes(void)
	{
		return changes_from_writer->getSeqNumListByStatus(CHANGE_FROM_WRITER_STATUS_KIND_MISSING);
	}

	void				WriterProxy::missing_changes_update(SequenceNumber_t last_available_seq_num)
	{
		//SequenceNumber_t	base	= changes_from_writer->getBaseNumber();
		SequenceNumber_t	base	= recent_available_seq;
		SequenceNumber_t	list_base	= changes_from_writer->getBaseNumber();
		ChangeFromWriter*	cfw		= NULL;

//printf("missing_base(%d, %d)\n", base.high, base.low);
//printf("missing_last(%d, %d)\n", last_available_seq_num.high, last_available_seq_num.low);

//printf("\tmissing_changes_update\n");
		if(base <= list_base)
		{
			base	= list_base;
		}

		while( (base <= last_available_seq_num) )
		{
			cfw	= changes_from_writer->getValueBySequenceNumber(base);

			if( (cfw) && (cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_UNKNOWN) )
			{
				changes_from_writer->changeStatusBySequenceNumber(base, CHANGE_FROM_WRITER_STATUS_KIND_MISSING);
			}
			else if( cfw == NULL )
			{
				cfw	= new ChangeFromWriter();
				cfw->status			= CHANGE_FROM_WRITER_STATUS_KIND_MISSING;
				cfw->is_relevant	= true;
#ifdef SHARED_PTR_CACHE_CHANGE
				cfw->cache_change	= pCacheChange(new CacheChange());
#else
				cfw->cache_change	= new CacheChange();
#endif

				cfw->cache_change->writer_guid.guid_prefix	= remote_writer_guid.guid_prefix;
				cfw->cache_change->writer_guid.entity_id	= remote_writer_guid.entity_id;
				cfw->cache_change->instance_handle			= 0;
				cfw->cache_change->kind						= CHANGE_KIND_ALIVE;
				cfw->cache_change->sequence_number			= base;
				cfw->cache_change->data_value				= NULL;

				while(!changes_from_writer->insertValue(cfw))
				{
					changes_from_writer->refreshByStatus();
				}
			}

			base	= base + SequenceNumber_t(0, 1);
		}
	}

	void				WriterProxy::received_change_set(SequenceNumber_t a_seq_num)
	{
		changes_from_writer->changeStatusBySequenceNumber(a_seq_num, CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED);
	}

	void				WriterProxy::start()
	{
		//printf("start()\n");
		if(is_running == false)
		{
			//printf("is_running == false \n");
			is_running	= true;
			if(((StatefulReader*)related_reader)->reliability_level == RELIABILITY_KIND_RELIABLE)
			{
				Message*		message					= NULL;
				EntityEvent*	entity_event;

				message	= (Message*)_create_a_acknack_message(&remote_writer_guid.entity_id, SequenceNumber_t(0,0), 0, NULL);

				entity_event			= new EntityEvent();
				entity_event->target	= (unsigned long)this;
				entity_event->event		= SEND_A_MESSAGE_TO_WRITER;
				entity_event->data		= (unsigned long)message->Serialize();

				//printf("Send a message!!\n");
				((StatefulReader*)related_reader)->SetEvent(entity_event);

				message->submessages->deleteAll();
			}
			else
			{
				//printf("RELIABILITY_KIND_BEST_EFFORT\n");
			}
		}
	}

	void*				WriterProxy::_create_a_acknack_message(EntityId_t* writer_id, SequenceNumber_t base, unsigned long num_bits, SequenceNumberSeq* nack_list)
	{
		Message*			message			= NULL;
		AckNack*			submessage		= NULL;
		StatefulReader*		reader			= (StatefulReader*)related_reader;
		ByteOrderingType	ordering		= SystemInformation::getSystemInformationInstance()->getSystemByteOrderingType();


		message							= new Message();
		message->header.guid_prefix		= reader->guid.guid_prefix;
		message->header.protocol		= PROTOCOL_RTPS;
		message->header.version.major	= PROTOCOL_VERSION.major;
		message->header.version.minor	= PROTOCOL_VERSION.minor;

		submessage									= new AckNack();
		submessage->submessage_header.flags			= 0x00;
		submessage->submessage_header.submessage_id	= SUBMESSAGE_KIND_ACKNACK;
		//submessage->writer_id.value					= reader->guid.entity_id;
		submessage->writer_id.value					= *writer_id;
		submessage->reader_id.value					= reader->guid.entity_id;
		submessage->count.value.value				= ack_count++;
		submessage->reader_sn_state.base			= base;
		submessage->reader_sn_state.numBits			= num_bits;

		while((nack_list != NULL) && (nack_list->getSize() > 0))
		{
			submessage->reader_sn_state.set.insertInRear(nack_list->popFirst());
		}

		message->submessages->insertInRear(submessage);

		return message;
	}

	void				WriterProxy::_create_a_cache_data(void* data_submessage, CacheChange* change)
	{
		Data*				submessage_data	= (Data*)data_submessage;

		change->data_value	= new ByteStream();

		if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_BE, 4))
		{
			change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_PL_CDR_BE, 4))
		{
			change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_LE, 4))
		{
			change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else
		{
			change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
	}

	ChangeFromWriter*	WriterProxy::_create_a_cachechange(void* data_submessage)
	{
		ChangeFromWriter*	cfw;
		Data*				submessage_data	= (Data*)data_submessage;

#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(submessage_data == NULL)printf("WriterProxy::_create_a_cachechange() E-001\n");
#endif
		cfw											= new ChangeFromWriter();
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw == NULL)printf("WriterProxy::_create_a_cachechange() E-002\n");
#endif
		cfw->is_relevant							= true;
		cfw->status									= CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED;

#ifdef SHARED_PTR_CACHE_CHANGE
		cfw->cache_change							= pCacheChange(new CacheChange());
#else
		cfw->cache_change							= new CacheChange();
#endif

#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw->cache_change == NULL)printf("WriterProxy::_create_a_cachechange() E-003\n");
#endif
		cfw->cache_change->writer_guid.guid_prefix	= remote_writer_guid.guid_prefix;
		cfw->cache_change->writer_guid.entity_id	= remote_writer_guid.entity_id;
		cfw->cache_change->instance_handle			= 0;
		cfw->cache_change->kind						= CHANGE_KIND_ALIVE;
		cfw->cache_change->sequence_number			= submessage_data->writer_sn.value;
		cfw->cache_change->data_value				= new ByteStream();
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw->cache_change->data_value == NULL)printf("WriterProxy::_create_a_cachechange() E-004\n");
if(submessage_data->serialized_payload.value.getData() == NULL)printf("WriterProxy::_create_a_cachechange() E-005\n");
if(submessage_data->serialized_payload.value.getLength() == 0)printf("WriterProxy::_create_a_cachechange() E-006\n");
#endif
		if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_BE, 4))
		{
			cfw->cache_change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_PL_CDR_BE, 4))
		{
			cfw->cache_change->data_value->setData(BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else if(!memcmp(submessage_data->serialized_payload.info, SERIALIZED_DATA_INFO_CDR_LE, 4))
		{
			cfw->cache_change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}
		else
		{
			cfw->cache_change->data_value->setData(BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST, submessage_data->serialized_payload.value.getData(), submessage_data->serialized_payload.value.getLength());
		}

		return cfw;
	}
/*
	void				WriterProxy::_chk_last_available_seq(SequenceNumber_t seq)
	{
		unsigned long		index;
		SequenceNumber_t	calc_seq;
		ChangeFromWriter*	cfw;

		if(last_available_seq < seq)
		{
			if(last_available_seq < changes_from_writer->getBaseNumber())
			{
				index		= 0;
			}
			else
			{
				calc_seq	= last_available_seq - changes_from_writer->getBaseNumber();
				index		= calc_seq.low;
			}

			do
			{
				index++;

				cfw		= changes_from_writer->getValueByIndex(index);
			}while((cfw != NULL) && ((cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED) || (cfw->status == CHANGE_FROM_WRITER_STATUS_KIND_LOST)));

			last_available_seq	= changes_from_writer->getValueByIndex(index-1)->cache_change->sequence_number;
		}
	}
*/
	void				WriterProxy::_after_hartbeat_response_delay(unsigned long arg)
	{
		WriterProxy*	proxy	= (WriterProxy*)arg;
		EntityEvent*	event	= new EntityEvent();

		//printf("_after_hartbeat_response_delay()\n");

		event->target	= arg;
		event->event	= AFTER_HEARTBEAT_RESONSE_DELAY;
		event->data		= 0;

		((StatefulReader*)proxy->related_reader)->SetEvent(event);
	}

	void				WriterProxy::set_event(ReaderEvent event, unsigned long data)
	{
		//printf("WriterProxy(%08X)::set_event(ws:%d, rs:%d, e:%d)\n",this, waiting_state, ready_state, event);

		Reader*				reader			= (Reader*)related_reader;
		CacheChange*		change			= NULL;
		ChangeFromWriter*	cfw				= NULL;
		EntityEvent*		entity_event	= NULL;

		Message*			message					= NULL;
		Data*				submessage_data			= NULL;
		Heartbeat*			submessage_heartbeat	= NULL;
		EntityId_t			writer_id;
		SequenceNumber_t	seq_num;
		SequenceNumber_t 	calc_num;
		SequenceNumberSeq*	seq_num_set				= NULL;

		DynamicTimer*		timer			= NULL;
		TimeUnit*			time_unit		= NULL;

		ListenerProcessor*	processor		= ListenerProcessor::getListenerProcessorInstance();
		Event*				listener_event	= NULL;

		if(reader->reliability_level == RELIABILITY_KIND_BEST_EFFORT) /* BEST_EFFORT *** BEST_EFFORT *** BEST_EFFORT *** BEST_EFFORT  *** BEST_EFFORT  *** BEST_EFFORT */
		{
			switch(event)
			{
			case DATA_IS_RECEIVED:	// Best-effort T2

				if(waiting_state == READER_STATE_WAITING)
				{
					//printf("WriterProxy(%08X) Best-effort T2\n", this);
					/*
					 cc	= new CacheChange(DATA);
					 writer_guid	= {Receiver.SourceGuidPrefix, DATA.writerId};
					 Writer_proxy	= the_rtps_reader.matched_writer_lookup(writer_guid);
					 expected_seq_num	= writer_proxy.available_changes_max() + 1;
					 if ( cc.sequenceNumber >= expected_seq_num)
					 {
					 	 the_rtps_reader.reader_cache.add_change(cc);
					 	 writer_proxy.received_change_set(cc.sequenceNumber);
					 	 if ( cc.sequenceNumber > expected_seq_num )
					 	 {
					 	 	 writer_proxy.lost_changes_update(cc.sequenceNumber);
					 	 }
					 }
					 */

					message	= (Message*)data;
					submessage_data	= (Data*)message->submessages->getFirst();

					seq_num	= available_changes_max() + SequenceNumber_t(0, 1);
					if( seq_num <= submessage_data->writer_sn.value )
					{
						cfw	= changes_from_writer->getValueBySequenceNumber(submessage_data->writer_sn.value);
						//_chk_last_available_seq(cfw->cache_change->sequence_number);

						if(cfw)
						{
#ifdef SHARED_PTR_CACHE_CHANGE
							_create_a_cache_data((void*)submessage_data, cfw->cache_change.get());
#else
							_create_a_cache_data((void*)submessage_data, cfw->cache_change);
#endif
							received_change_set(submessage_data->writer_sn.value);
						}
						else
						{
							cfw	= _create_a_cachechange(submessage_data);

							while(!changes_from_writer->insertValue(cfw))
							{
								changes_from_writer->refreshByStatus();
							}

							reader->reader_cache->add_change(cfw->cache_change);

							listener_event	= new Event();
							listener_event->type	= EVENT_DATA_AVAILABLE;
							listener_event->arg		= (unsigned long)reader->related_dcps_reader;
							processor->setEvent(listener_event);
						}
					}
				}

				break;

			case DELETE_WRITER_PROXY:	// Best-effort T3
				break;

			default:
				break;
			}
		}
		else if(reader->reliability_level == RELIABILITY_KIND_RELIABLE) /* RELIABLE *** RELIABLE *** RELIABLE *** RELIABLE  *** RELIABLE  *** RELIABLE *** RELIABLE *** RELIABLE */
		{
			switch(event)
			{
			case DATA_IS_RECEIVED:	// Reliable T8

				if( ready_state == READER_STATE_READY )
				{
					//printf("WriterProxy(%08X) Reliable T8\n", this);
					/*
					 cc	= new CacheChange(DATA);
					 the_reader.reader_cache.add_change(cc);
					 the_writer_proxy.received_change_set(cc.sequenceNumber);
					 */
					message	= (Message*)data;
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(message == NULL)printf("WriterProxy::set_event() E-004\n");
#endif
					submessage_data	= (Data*)message->submessages->getFirst();
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(submessage_data == NULL)printf("WriterProxy::set_event() E-005\n");
#endif
					seq_num	= submessage_data->writer_sn.value;

//printf("\tDATA_IS_RECEIVED(%d, %d)\n", submessage_data->writer_sn.value.high, submessage_data->writer_sn.value.low);

					cfw	= changes_from_writer->getValueBySequenceNumber(submessage_data->writer_sn.value);

					if(cfw == NULL)
					{
						cfw	= _create_a_cachechange((void*)submessage_data);
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw == NULL)	printf("WriterProxy::set_event() E-001\n");
#endif
						while(!changes_from_writer->insertValue(cfw))
						{
//printf("WriterProxy::changes_from_writer->refreshByStatus()\n");
							changes_from_writer->refreshByStatus();
						}
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw->cache_change == NULL)printf("WriterProxy::set_event() E-002\n");
#endif
						reader->reader_cache->add_change(cfw->cache_change);

						listener_event			= new Event();
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(listener_event == NULL)printf("WriterProxy::set_event() E-003\n");
#endif
						listener_event->type	= EVENT_DATA_AVAILABLE;
						listener_event->arg		= (unsigned long)reader->related_dcps_reader;
						processor->setEvent(listener_event);
					}
					else if(cfw->status != CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED)
					{
//printf("[NOT] CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED\n");
#ifdef SHARED_PTR_CACHE_CHANGE
						_create_a_cache_data((void*)submessage_data, cfw->cache_change.get());
#else
						_create_a_cache_data((void*)submessage_data, cfw->cache_change);
#endif
						received_change_set(submessage_data->writer_sn.value);
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(cfw->cache_change == NULL)printf("WriterProxy::set_event() E-006\n");
#endif
						reader->reader_cache->add_change(cfw->cache_change);
						listener_event	= new Event();
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(listener_event == NULL)printf("WriterProxy::set_event() E-007\n");
#endif
						listener_event->type	= EVENT_DATA_AVAILABLE;
						listener_event->arg		= (unsigned long)reader->related_dcps_reader;
						processor->setEvent(listener_event);
					}
					else
					{
//fprintf(fp, "data_is_received,same,%d,%d,%d,%d\n", cfw->status, CHANGE_FROM_WRITER_STATUS_KIND_RECEIVED, cfw->cache_change->sequence_number.high, cfw->cache_change->sequence_number.low);
//fflush(fp);
						/*
						printf("A suspicion point\n");
						*/
					}

					message->submessages->deleteAll();
					delete(message);
				}
				else
				{
					//printf("Data message is received when ready_state is (%d)\n", ready_state);
				}

				break;

			case HEARTBEAT_IS_RECEIVED:	// Reliable T2, T7

				if( ready_state == READER_STATE_READY )	// Reliable T7
				{
					//printf("WriterProxy(%08X) Reliable T7\n", this);

					message	= (Message*)data;
					submessage_heartbeat	= (Heartbeat*)message->submessages->getFirst();

					if(submessage_heartbeat->last_sn.value.low > 0)
					{
//printf("\tT7-1\n");
						missing_changes_update(submessage_heartbeat->last_sn.value);
//printf("\tT7-2\n");
						lost_changes_update(submessage_heartbeat->first_sn.value);
//printf("\tT7-3\n");
					}
					else
					{
						//printf("submessage_heartbeat->last_sn.value.low == 0\n");
					}
				}

				if( waiting_state == READER_STATE_WAITING )	// Reliable T2
				{
					//printf("WriterProxy(%08X) Reliable T2\n", this);

					message	= (Message*)data;
					submessage_heartbeat	= (Heartbeat*)message->submessages->getFirst();

					if((submessage_heartbeat->submessage_header.flags & 0x02) == 0x00)	//HEARTBEAT.FinalFlag == NOT_SET
					{
						waiting_state	= READER_STATE_MUST_SEND_ACK;

						//  register TIMER for ACKNACK
						timer	= DynamicTimer::getDynamicTimerInstance();

						time_unit	= new TimeUnit(reader->heartbeat_response_delay.getDurationByMilliSecond());
						time_unit->setDataValue((unsigned long)this);
						time_unit->setExecuteFunction(WriterProxy::_after_hartbeat_response_delay);

						timer->regist(time_unit);
					}
					else if(submessage_heartbeat->submessage_header.flags & 0x04 == 0x00)	//HEARTBEAT.LivelinessFlag == NOT_SET
					{
						waiting_state	= READER_STATE_MAY_SEND_ACK;

						cfw	= changes_from_writer->getFirstValueByStatus(CHANGE_FROM_WRITER_STATUS_KIND_MISSING);
						if(cfw == NULL)
						{
							entity_event			= new EntityEvent();
							entity_event->target	= (unsigned long)this;
							entity_event->event		= MISSING_CHANGES_FALSE;
							entity_event->data		= 0;

							reader->SetEvent(entity_event);
						}
						else
						{
							entity_event			= new EntityEvent();
							entity_event->target	= (unsigned long)this;
							entity_event->event		= MISSING_CHANGES_TRUE;
							entity_event->data		= 0;

							reader->SetEvent(entity_event);
						}
					 }
					 else
					 {
					 	 // do nothing
					 }
				}
				message->submessages->deleteAll();
				delete(message);
				break;

			case GAP_IS_RECEIVED:	// Reliable T9

				if( ready_state == READER_STATE_READY )
				{
					//printf("WriterProxy(%08X) Reliable T9\n", this);
					//_chk_last_available_seq(GAP_SEQ);
					/*
					 FOREACH seq_num IN [GAP.gapStart, GAP.gapList.base-1]
					 DO{
					 	 the_writer_proxy.irrelevant_change_set(seq_num);
					 };

					 FOREACH seq_num IN GAP.gapList
					 DO{
					 	 the_writer_proxy.irrelevant_change_set(seq_num);
					 };
					 */

					message->submessages->deleteAll();
					delete(message);
				}

				break;

			case MISSING_CHANGES_TRUE:	// Reliable T4

				if( waiting_state == READER_STATE_MAY_SEND_ACK )
				{
					//printf("WriterProxy(%08X) Reliable T4\n", this);

					waiting_state	= READER_STATE_MUST_SEND_ACK;

					//  register TIMER for ACKNACK
					timer	= DynamicTimer::getDynamicTimerInstance();

					time_unit	= new TimeUnit(reader->heartbeat_response_delay.getDurationByMilliSecond());
					time_unit->setDataValue((unsigned long)this);
					time_unit->setExecuteFunction(WriterProxy::_after_hartbeat_response_delay);
				}

				break;

			case MISSING_CHANGES_FALSE:	// Reliable T3

				if( waiting_state == READER_STATE_MAY_SEND_ACK )
				{
					//printf("WriterProxy(%08X) Reliable T3\n", this);

					waiting_state	= READER_STATE_WAITING;
				}

				break;

			case AFTER_HEARTBEAT_RESONSE_DELAY:	// Reliable T5
				//printf("WriterProxy(%08X) AFTER_HB_RD\n");
				if( waiting_state == READER_STATE_MUST_SEND_ACK )
				{
					//printf("WriterProxy(%08X) Reliable T5\n", this);
					/*
					 missing_seq_num_set.base	= the_writer_proxy.available_changes_max() + 1;
					 missing_seq_num_set.set	= <empty>;
					 FOREACH change IN the_writer_proxy.missing_changes()
					 DO{
					 	 ADD change.sequenceNumber TO missing_seq_num_set.set;
					 }
					 send ACKNACK(missing_seq_num_set);
					 */

					waiting_state	= READER_STATE_WAITING;

					seq_num		= available_changes_max() + SequenceNumber_t(0, 1);
					seq_num_set	= missing_changes();
					calc_num	= SequenceNumber_t(0,0);

					if(seq_num_set)
					{
						calc_num	= (*seq_num_set->getLast()) - (*seq_num_set->getFirst());
						calc_num	= calc_num	+ SequenceNumber_t(0,1);
//printf("WriterProxy::calc_num(%d,%d)\n", calc_num.high, calc_num.low);
					}/*
					else
					{
						printf("hit\n");
						seq_num.high	= 0;
						seq_num.low		= 1;
					}*/
//printf("missing_changes_seq_num(%d, %d) seq_num_set(%08X)\n", seq_num.high, seq_num.low, seq_num_set);
//printf("missing_changes_base(%d, %d)\n", seq_num.high, seq_num.low);

					message	= (Message*)_create_a_acknack_message(&remote_writer_guid.entity_id, seq_num, calc_num.low, seq_num_set);

					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= SEND_A_MESSAGE_TO_WRITER;
					entity_event->data		= (unsigned long)message->Serialize();

					reader->SetEvent(entity_event);

					message->submessages->deleteAll();
					delete(message);
				}

				break;

			case DELETE_WRITER_PROXY:	// Reliable T10
				break;

			default:
				break;
			}
		}
		else
		{
			// error
		}

		//printf("WriterProxy(%08X)::clear(e:%d)\n", this, event);
	}
}
