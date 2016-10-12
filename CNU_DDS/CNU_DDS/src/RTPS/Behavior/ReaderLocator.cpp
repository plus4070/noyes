#include "../../../include/RTPS/Behavior/ReaderLocator.h"
#include "../../../include/RTPS/Behavior/StatelessWriter.h"
#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessages/Submessages.h"

#include <stdio.h>

namespace CNU_DDS
{
	ReaderLocator::ReaderLocator(unsigned long writer_instance, Locator_t reader_locator, bool a_expects_inline_qos)
	{
		unsent_changes		= new CacheChangeSeq();
		requested_changes	= new CacheChangeSeq();
		expects_inline_qos	= a_expects_inline_qos;
		locator				= reader_locator;
		related_writer		= writer_instance;

		if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			pushing_state		= WRITER_STATE_IDLE;
		}
		else if(((Writer*)writer_instance)->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			pushing_state		= WRITER_STATE_ANNOUNCING;
			repairing_state		= WRITER_STATE_WAITING;
		}
		else
		{
			//error
		}
	}

	ReaderLocator::~ReaderLocator()
	{
		unsent_changes->deleteAllNode();
		requested_changes->deleteAllNode();

		delete(unsent_changes);
		delete(requested_changes);
	}

	pCacheChange	ReaderLocator::next_requested_change(void)
	{
		return	requested_changes->popFirst();
	}

	pCacheChange	ReaderLocator::next_unsent_change(void)
	{
		return	unsent_changes->popFirst();
	}

	void			ReaderLocator::requested_changes_set(SequenceNumberSeq* req_seq_num_set)
	{
		/*
		 * I guess this function is not used.
		 * A ReaderLocator does not know the CacheChange list which it's writer has.
		 * by Gundoll (2015.03.02)
		 * */
	}

	void*			ReaderLocator::_create_a_data_message(CacheChange* change)
	{
		Message*			message			= new Message();
		Data*				submessage_data	= new Data();
		StatelessWriter*	writer			= (StatelessWriter*)related_writer;

		message					= new Message();
		message->header.guid_prefix		= writer->guid.guid_prefix;
		message->header.protocol		= PROTOCOL_RTPS;
		message->header.version.major	= PROTOCOL_VERSION.major;
		message->header.version.minor	= PROTOCOL_VERSION.minor;

		submessage_data			= new Data();
		submessage_data->submessage_header.submessage_id	= SUBMESSAGE_KIND_DATA;
		submessage_data->submessage_header.flags			= 0x04;
		submessage_data->writer_id.value					= writer->guid.entity_id;
		submessage_data->reader_id.value					= ENTITYID_UNKNOWN;
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

	void		ReaderLocator::set_event(WriterEvent event, unsigned long data)
	{
		StatelessWriter*	writer				= (StatelessWriter*)related_writer;
		pCacheChange		change				= NULL;
		EntityEvent*		entity_event		= NULL;

		Message*			message				= NULL;
		Gap*				submessage_gap		= NULL;
		AckNack*			submessage_acknack	= NULL;

		if(writer->reliability_level == RELIABILITY_KIND_BEST_EFFORT)
		{
			switch(event)
			{
			case UNSENT_CHANGES_TRUE:

				if( pushing_state == WRITER_STATE_IDLE )
				{
					pushing_state	= WRITER_STATE_PUSHING;

					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= CAN_PUSHING;
					entity_event->data		= 0;

					writer->SetEvent(entity_event);
				}

				break;

			case UNSENT_CHANGES_FALSE:

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					pushing_state	= WRITER_STATE_IDLE;
				}

				break;

			case CAN_PUSHING:

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					change	= next_unsent_change();

					if( change == NULL )
					{
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else
					{
						// send DATA | GAP
						/*
						 DATA	= new DATA(change);
						 IF (the_reader_locator.expectsInlineQos)
						 {
						 	 DATA.inlineQoS = the_writer.related_dds_wirter.qos;
						 }
						 DATA.readerId	= ENTITYID_UNKNOWN;
						 sendto the_reader_locator.locator, DATA;
						 */

#ifdef SHARED_PTR_CACHE_CHANGE
						message					= (Message*)_create_a_data_message(change.get());
#else
						message					= (Message*)_create_a_data_message(change);
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
				}

				break;

			case DELETE_READER_LOCATOR:
				break;

			default:
				break;
			}
		}
		else if( writer->reliability_level == RELIABILITY_KIND_RELIABLE )
		{
			switch(event)
			{
			case UNSENT_CHANGES_TRUE:

				if( pushing_state == WRITER_STATE_ANNOUNCING )
				{
					entity_event			= new EntityEvent();
					entity_event->target	= (unsigned long)this;
					entity_event->event		= CAN_PUSHING;
					entity_event->data		= 0;

					writer->SetEvent(entity_event);
				}

				break;

			case UNSENT_CHANGES_FALSE:

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					pushing_state	= WRITER_STATE_ANNOUNCING;
				}

				break;

			case CAN_PUSHING:

				if( pushing_state == WRITER_STATE_PUSHING )
				{
					change	= next_unsent_change();

					if( change == NULL )
					{
						entity_event			= new EntityEvent();
						entity_event->target	= (unsigned long)this;
						entity_event->event		= UNSENT_CHANGES_FALSE;
						entity_event->data		= 0;

						writer->SetEvent(entity_event);
					}
					else
					{
						// send DATA | GAP
						/*
						 DATA	= new DATA(change);
						 IF ( the_reader_locator.expectsInlineQos )
						 {
						 	 DATA.inlineQos	= the_writer.related_dds_writer.qos;
						 }
						 DATA.readerId	= ENTITYID_UNKNOWN;
						 sendto the_reader_locator.locator, DATA;
						 */

#ifdef SHARED_PTR_CACHE_CHANGE
						message					= (Message*)_create_a_data_message(change.get());
#else
						message					= (Message*)_create_a_data_message(change);
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
				}

				break;

			case ACKNACK_IS_RECEIVED:

				if( repairing_state == WRITER_STATE_WAITING )
				{
					/*
					 FOREACH reply_locator_t IN { Receiver.unicastReplyLocatorList, Receiver.multicastReplyLocatorList }
					 reader_locator	= the_rtps_writer.reader_locator_lookup(reply_locator_t);
					 reader_locator.requested_changes_set (ACKNACK.readerSNstate.set)
					 */

					message->submessages->deleteAll();
					delete(message);
				}
				else if ( repairing_state == WRITER_STATE_MUST_REPAIR )
				{
					/*
					 FOREACH reply_locator_t IN { Receiver.unicastReplyLocatorList, Receiver.multicastReplyLocatorList }
					 reader_locator	= the_rtps_writer.reader_locator_lookup(reply_locator_t);
					 reader_locator.requested_changes_set (ACKNACK.readerSNstate.set)
					 */

					message->submessages->deleteAll();
					delete(message);
				}

				break;

			case CAN_REPAIRING:
				change	= next_requested_change();

				if( change == NULL )
				{
					// set event REQUESTED_CHANGES_FALSE to related writer
				}
				else
				{
					// send DATA | GAP
					/*
					 IF change IN the_writer.writer_cache.changes
					 {
					 	 DATA	= new DATA(change);

					 	 IF ( the_reader_locator.expectedInlineQos )
					 	 {
					 	 	 DATA.inlineQos	= the_writer.related_dds_writer.qos;
					 	 }
					 	 DATA.readerId	= ENTITYID_UNKNOWN;
					 	 sendto the_reader_locator, DATA;
					 }
					 ELSE
					 {
					 	 GAP	= new GAP(a_change.sequenceNumber);
					 	 GAP.readerId	= ENTITYID_UNKNOWN;
					 	 sendto the_reader_locator, GAP;
					 }
					 */
				}

				break;

			case REQUESTED_CHANGES_TRUE:

				if( repairing_state == WRITER_STATE_WAITING )
				{
					repairing_state	= WRITER_STATE_MUST_REPAIR;
				}

				break;

			case REQUESTED_CHANGES_FALSE:

				if( repairing_state == WRITER_STATE_REPAIRING )
				{
					repairing_state	= WRITER_STATE_WAITING;
				}

				break;

			case AFTER_HEARTBEAT_PERIOD:

				if( pushing_state == WRITER_STATE_ANNOUNCING )
				{
					// send HEARTBEAT(FinalFalg=SET)
				}

				break;

			case AFTER_NACK_RESPONSE_DELAY:

				if( repairing_state == WRITER_STATE_MUST_REPAIR )
				{
					repairing_state	= WRITER_STATE_REPAIRING;
				}

				break;

			case DELETE_READER_LOCATOR:
				break;

			default:
				break;
			}
		}
		else
		{
			//error
		}
	}
}
