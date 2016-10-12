#include "../../../include/RTPS/Behavior/ListenerProcessor.h"
#include "../../../include/DCPS/Infrastructure/Entity.h"
#include "../../../include/DCPS/Topic/TopicListener.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/DCPS/Domain/DomainParticipantListener.h"
#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/PublisherListener.h"
#include "../../../include/DCPS/Publication/DataWriter.h"
#include "../../../include/DCPS/Publication/DataWriterListener.h"
#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/DCPS/Subscription/SubscriberListener.h"
#include "../../../include/DCPS/Subscription/DataReader.h"
#include "../../../include/DCPS/Subscription/DataReaderListener.h"
#include "../../../include/Status/EntityStatus/DataReaderStatus.h"
#include "../../../include/Status/EntityStatus/DataWriterStatus.h"
#include "../../../include/Status/EntityStatus/DomainParticipantStatus.h"
#include "../../../include/Status/EntityStatus/PublisherStatus.h"
#include "../../../include/Status/EntityStatus/SubscriberStatus.h"
#include "../../../include/Status/EntityStatus/TopicStatus.h"
#include <stdio.h>

namespace CNU_DDS
{
	ListenerProcessor*	LISTENER_PROCESSOR_INSTANCE;

	ListenerProcessor::ListenerProcessor():EventProcessor()
	{

	}

	ListenerProcessor::~ListenerProcessor()
	{

	}

	ListenerProcessor*	ListenerProcessor::getListenerProcessorInstance()
	{
		if(LISTENER_PROCESSOR_INSTANCE == NULL)
		{
			LISTENER_PROCESSOR_INSTANCE	= new ListenerProcessor();
			LISTENER_PROCESSOR_INSTANCE->start();
		}

		return LISTENER_PROCESSOR_INSTANCE;
	}

	void	ListenerProcessor::process()
	{
		Event*	event		= NULL;

		if(this->events->getSize() <= 0)
		{
			this->waitEvents();
		}

		event		= events->popFirst();
		if(event == NULL)
		{
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
printf("ListenerProcessor::process() E-001\n");
printf("\tevents->size == \n", events->getSize());
#endif
			return;
		}
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(event == NULL)printf("ListenerProcessor::process() E-001\n");
#endif

		switch(event->type)
		{
			case EVENT_INCONSISTENT_TOPIC:
				_inconsistent_topic(event->arg);
				break;
			case EVENT_SAMPLE_REJECTED:
				_sample_rejected(event->arg);
				break;
			case EVENT_LIVELINESS_CHANGED:
				_liveliness_changed(event->arg);
				break;
			case EVENT_REQUESTED_DEADLINE_MISSED:
				_requested_deadline_missed(event->arg);
				break;
			case EVENT_REQUESTED_INCOMPATIBLE_QOS:
				_requested_incompatible_qos(event->arg);
				break;
			case EVENT_DATA_AVAILABLE:
				if(_data_on_readers(event->arg) == false)
				{
					_data_available(event->arg);
				}
				break;
			case EVENT_SAMPLE_LOST:
				_sample_lost(event->arg);
				break;
			case EVENT_SUBSCRIPTION_MATCHED:
				_subscription_matched(event->arg);
				break;
			case EVENT_OFFERED_DEADLINE_MISSED:
				_offered_deadline_missed(event->arg);
				break;
			case EVENT_OFFERED_INCOMPATIBLE_QOS:
				_offered_incompatible_qos(event->arg);
				break;
			case EVENT_LIVELINESS_LOST:
				_liveliness_lost(event->arg);
				break;
			case EVENT_PUBLICATION_MATCHED:
				_publication_matched(event->arg);
				break;
			default:
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
printf("ListenerProcessor::process() E-002\n");
#endif
				break;
		}

		delete(event);
	}

	bool	ListenerProcessor::_is_enabled(unsigned long arg, StatusKind kind)
	{
		Entity*	entity	= (Entity*) arg;

		if(entity->get_status_condition()->enabled_statuses & kind)
		{
			return true;
		}
		return false;
	}

	bool	ListenerProcessor::_inconsistent_topic(unsigned long arg)
	{
		Topic*				topic		= (Topic*)arg;
		TopicStatus*		status		= (TopicStatus*)topic->status;
		TopicListener*		listener	= (TopicListener*)topic->get_listener();

		if(listener && _is_enabled(arg, INCONSISTENT_TOPIC_STATUS))
		{
			listener->on_inconsistent_topic(topic, &status->inconsistent_topic);
			return true;
		}
		return false;
	}

	bool	ListenerProcessor::_data_on_readers(unsigned long arg)
	{
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(arg == 0)printf("ListenerProcessor::_data_on_readers() E-001\n");
#endif
		Subscriber*			sub			= ((DataReader*)arg)->related_subscriber;
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(sub == NULL)printf("ListenerProcessor::_data_on_readers() E-002\n");
#endif
		SubscriberListener*	listener	= (SubscriberListener*)sub->get_listener();

		if(listener && _is_enabled((unsigned long)arg, DATA_ON_READERS_STATUS))
		{
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(listener == NULL)printf("ListenerProcessor::_data_on_readers() E-003\n");
#endif
			listener->on_data_on_readers((pSubscriber)sub);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_sample_rejected(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, SAMPLE_REJECTED_STATUS))
		{
			sub_listener->on_sample_rejected(reader, &status->sample_rejected);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, SAMPLE_REJECTED_STATUS))
		{
			reader_listener->on_sample_rejected(reader, &status->sample_rejected);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_liveliness_changed(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, LIVELINESS_CHANGED_STATUS))
		{
			sub_listener->on_liveliness_changed(reader, &status->liveliness_changed);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, LIVELINESS_CHANGED_STATUS))
		{
			reader_listener->on_liveliness_changed(reader, &status->liveliness_changed);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_requested_deadline_missed(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, REQUESTED_DEADLINE_MISSED_STATUS))
		{
			sub_listener->on_requested_deadline_missed(reader, &status->requested_deadline_missed);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, REQUESTED_DEADLINE_MISSED_STATUS))
		{
			reader_listener->on_requested_deadline_missed(reader, &status->requested_deadline_missed);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_requested_incompatible_qos(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, REQUESTED_INCOMPATIBLE_QOS_STATUS))
		{
			sub_listener->on_requested_incompatible_qos(reader, &status->requested_incompatile_qos);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, REQUESTED_INCOMPATIBLE_QOS_STATUS))
		{
			reader_listener->on_requested_incompatible_qos(reader, &status->requested_incompatile_qos);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_data_available(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(reader == NULL)printf("ListenerProcessor::_data_available() E-001\n");
#endif
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(status == NULL)printf("ListenerProcessor::_data_available() E-002\n");
#endif
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();
		Subscriber*			sub				= reader->related_subscriber;
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(sub == 0)printf("ListenerProcessor::_data_available() E-003\n");
#endif
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, DATA_AVAILABLE_STATUS))
		{
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(sub_listener == NULL)printf("ListenerProcessor::_data_available() E-004\n");
#endif
			sub_listener->on_data_available(reader);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, DATA_AVAILABLE_STATUS))
		{
#ifdef NULL_POINTER_EXCEPTION_DEBUGGING_MODE
if(reader_listener == NULL)printf("ListenerProcessor::_data_available() E-005\n");
#endif
			reader_listener->on_data_available(reader);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_sample_lost(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, SAMPLE_LOST_STATUS))
		{
			sub_listener->on_sample_lost(reader, &status->sample_lost);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, SAMPLE_LOST_STATUS))
		{
			reader_listener->on_sample_lost(reader, &status->sample_lost);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_subscription_matched(unsigned long arg)
	{
		DataReader*			reader			= (DataReader*)arg;
		DataReaderStatus*	status			= (DataReaderStatus*)reader->status;
		DataReaderListener*	reader_listener	= (DataReaderListener*)reader->get_listener();

		Subscriber*			sub				= reader->related_subscriber;
		SubscriberListener*	sub_listener	= (SubscriberListener*)sub->get_listener();

		if(sub_listener && _is_enabled((unsigned long)sub, SUBSCRIPTION_MATCHED_STATUS))
		{
			sub_listener->on_subscription_matched(reader, &status->subscription_matched);
			return true;
		}
		if(reader_listener && _is_enabled((unsigned long)reader, SUBSCRIPTION_MATCHED_STATUS))
		{
			reader_listener->on_subscription_matched(reader, &status->subscription_matched);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_offered_deadline_missed(unsigned long arg)
	{
		DataWriter*			writer			= (DataWriter*)arg;
		DataWriterStatus*	status			= (DataWriterStatus*)writer->status;
		DataWriterListener*	writer_listener	= (DataWriterListener*)writer->get_listener();

		Publisher*			pub				= writer->related_publisher;
		PublisherListener*	pub_listener	= (PublisherListener*)pub->get_listener();

		if(pub_listener && _is_enabled((unsigned long)pub, OFFERED_DEADLINE_MISSED_STATUS))
		{
			pub_listener->on_offered_deadline_missed(writer, &status->offered_deadline_missed);
			return true;
		}
		if(writer_listener && _is_enabled((unsigned long)writer, OFFERED_DEADLINE_MISSED_STATUS))
		{
			writer_listener->on_offered_deadline_missed(writer, &status->offered_deadline_missed);
			return true;
		}

		return false;
	}

	bool	ListenerProcessor::_offered_incompatible_qos(unsigned long arg)
	{
		DataWriter*			writer			= (DataWriter*)arg;
		DataWriterStatus*	status			= (DataWriterStatus*)writer->status;
		DataWriterListener*	writer_listener	= (DataWriterListener*)writer->get_listener();

		Publisher*			pub				= writer->related_publisher;
		PublisherListener*	pub_listener	= (PublisherListener*)pub->get_listener();

		if(pub_listener && _is_enabled((unsigned long)pub, OFFERED_INCOMPATIBLE_QOS_STATUS))
		{
			pub_listener->on_offered_incompatible_qos(writer, &status->offered_incompatible_qos);
			return true;
		}
		if(writer_listener && _is_enabled((unsigned long)writer, OFFERED_INCOMPATIBLE_QOS_STATUS))
		{
			writer_listener->on_offered_incompatible_qos(writer, &status->offered_incompatible_qos);
			return true;
		}
		return false;
	}

	bool	ListenerProcessor::_liveliness_lost(unsigned long arg)
	{
		DataWriter*			writer			= (DataWriter*)arg;
		DataWriterStatus*	status			= (DataWriterStatus*)writer->status;
		DataWriterListener*	writer_listener	= (DataWriterListener*)writer->get_listener();

		Publisher*			pub				= writer->related_publisher;
		PublisherListener*	pub_listener	= (PublisherListener*)pub->get_listener();

		if(pub_listener && _is_enabled((unsigned long)pub, LIVELINESS_LOST_STATUS))
		{
			pub_listener->on_liveliness_lost(writer, &status->liveliness_lost);
			return true;
		}
		if(writer_listener && _is_enabled((unsigned long)writer, LIVELINESS_LOST_STATUS))
		{
			writer_listener->on_liveliness_lost(writer, &status->liveliness_lost);
			return true;
		}
		return false;
	}

	bool	ListenerProcessor::_publication_matched(unsigned long arg)
	{
		DataWriter*			writer			= (DataWriter*)arg;
		DataWriterStatus*	status			= (DataWriterStatus*)writer->status;
		DataWriterListener*	writer_listener	= (DataWriterListener*)writer->get_listener();

		Publisher*			pub				= writer->related_publisher;
		PublisherListener*	pub_listener	= (PublisherListener*)pub->get_listener();

		if(pub_listener && _is_enabled((unsigned long)pub, PUBLICATION_MATCHED_STATUS))
		{
			pub_listener->on_publication_matched(writer, &status->publication_matched);
			return true;
		}
		if(writer_listener && _is_enabled((unsigned long)writer, PUBLICATION_MATCHED_STATUS))
		{
			writer_listener->on_publication_matched(writer, &status->publication_matched);
			return true;
		}
		return false;
	}
}
