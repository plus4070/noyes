#include "../../../include/RTPS/Behavior/StatelessWriter.h"
#include "../../../include/Utility/Network/TransceiverManager.h"

#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/DataWriter.h"

#include <stdio.h>

namespace CNU_DDS
{
	StatelessWriter::StatelessWriter(GUID_t a_guid, ReliabilityKind_t reliability, Duration_t a_resend_data_period)
	{
		this->guid.entity_id		= a_guid.entity_id;
		this->guid.guid_prefix		= a_guid.guid_prefix;
		this->reliability_level		= reliability;

		resend_data_period	= a_resend_data_period;
		reader_locators		= new ReaderLocatorSeq();

		is_running			= false;
	}

	StatelessWriter::StatelessWriter(Duration_t a_resend_data_period)
	{
		resend_data_period	= a_resend_data_period;
		reader_locators		= new ReaderLocatorSeq();
	}

	StatelessWriter::~StatelessWriter()
	{
		destroy_thread(&processing_thread);

		delete(reader_locators);
		delete(events);
	}

	void			StatelessWriter::start()
	{
		if(is_running == false)
		{
			is_running	= true;
			create_thread(&processing_thread, StatelessWriter::processing, this);
		}
	}

	void	StatelessWriter::reader_locator_add(Locator_t a_locator)
	{
		ReaderLocator*	new_locator	= NULL;
		new_locator	= new ReaderLocator((unsigned long)this, a_locator, false);

		reader_locators->insertInRear(new_locator);
	}

	void	StatelessWriter::reader_locator_remove(Locator_t a_locator)
	{
		Node<ReaderLocator>*	head		= reader_locators->getNodeByIndex(0);
		Node<ReaderLocator>*	cur_node	= head->rear;

		reader_locators->listLock();
		while(cur_node != head)
		{
			if( (cur_node != NULL) && (cur_node->value->locator == a_locator) )
			{
				reader_locators->listUnlock();
				reader_locators->cutNode(cur_node);
				reader_locators->listLock();
				delete(cur_node);
				break;
			}

			cur_node	= cur_node->rear;
		}
		reader_locators->listUnlock();
	}

	void	StatelessWriter::unsent_changes_reset(void)
	{
		Node<ReaderLocator>*	head		= reader_locators->getNodeByIndex(0);
		Node<ReaderLocator>*	cur_node	= head->rear;
		ReaderLocator*			locator;
		EntityEvent*			event;
		int						i;

		reader_locators->listLock();
		while(cur_node != head)
		{
			reader_locators->listUnlock();
			if(cur_node != NULL)
			{
				for(i=1; i<=writer_cache->changes->getSize(); i++)
				{
					cur_node->value->unsent_changes->insertInRear(writer_cache->changes->getNodeByIndex(i)->value);
				}
			}

			event			= new EntityEvent();
			event->data		= 0;
			event->target	= (unsigned long)cur_node->value;
			event->event	= UNSENT_CHANGES_TRUE;

			this->SetEvent(event);

			reader_locators->listLock();

			cur_node	= cur_node->rear;
		}
		reader_locators->listUnlock();
	}

	CacheChange*		StatelessWriter::new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle)
	{
		CacheChange*	result;
		int				i;
		ReaderLocator*	locator;
		EntityEvent*	event;
#ifdef SHARED_PTR_CACHE_CHANGE
		pCacheChange	change;
#endif

		last_change_sequence_number	+= SequenceNumber_t(0,1);

		result					= new CacheChange();
		result->kind			= kind;
		memcpy(&result->writer_guid, &this->guid, sizeof(GUID_t));
		result->sequence_number	= last_change_sequence_number;
		result->data_value		= data;
		result->instance_handle	= handle;

#ifdef SHARED_PTR_CACHE_CHANGE
		change					= pCacheChange(result);
		writer_cache->add_change(change);
#else
		writer_cache->add_change(result);
#endif

		for(i=1; i<=reader_locators->getSize(); i++)
		{
			locator	= reader_locators->getNodeByIndex(i)->value;
#ifdef SHARED_PTR_CACHE_CHANGE
			locator->unsent_changes->insertInRear(change);
#else
			locator->unsent_changes->insertInRear(result);
#endif

			event			= new EntityEvent();
			event->data		= 0;
			event->target	= (unsigned long)locator;
			event->event	= UNSENT_CHANGES_TRUE;

			this->SetEvent(event);
		}

		return	result;
	}

	void*	StatelessWriter::_get_transceiver()
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

	void*	StatelessWriter::processing(void* arg)
	{
		StatelessWriter*	writer	= (StatelessWriter*)arg;
		EntityEvent*		event;
		ReaderLocator*		reader_locator;
		Transceiver*		transceiver;

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
						reader_locator	= (ReaderLocator*)event->target;
						transceiver->send((ByteStream*)event->data, reader_locator->locator);
						delete(event);
					}
					else
					{
						writer->_get_transceiver();
						writer->SetEvent(event);
					}
					break;

				default:
					reader_locator	= (ReaderLocator*)event->target;
					reader_locator->set_event((WriterEvent)event->event, event->data);

					delete(event);

					break;
				}
			}

			writer->Wait();
		}

		return 0;
	}
}
