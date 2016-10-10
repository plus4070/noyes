#include "../../../include/DCPS/Domain/DomainParticipant.h"

#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/DataWriter.h"
#include "../../../include/DCPS/Subscription/Subscriber.h"
#include "../../../include/DCPS/Subscription/DataReader.h"
#include "../../../include/DCPS/Topic/Topic.h"
#include "../../../include/DCPS/Topic/ContentFilteredTopic.h"
#include "../../../include/DCPS/Topic/MultiTopic.h"
#include "../../../include/Status/EntityStatus/DomainParticipantStatus.h"

#include "../../../include/DCPS/BuiltinEntity/BuiltinParticipantsReaderListener.h"
#include "../../../include/DCPS/BuiltinEntity/BuiltinParticipantsWriterListener.h"
#include "../../../include/DCPS/BuiltinEntity/BuiltinPublicationsReaderListener.h"
#include "../../../include/DCPS/BuiltinEntity/BuiltinPublicationsWriterListener.h"
#include "../../../include/DCPS/BuiltinEntity/BuiltinSubscriptionsReaderListener.h"
#include "../../../include/DCPS/BuiltinEntity/BuiltinSubscriptionsWriterListener.h"

#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Behavior/StatelessWriter.h"
#include "../../../include/RTPS/Behavior/StatelessReader.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"
#include "../../../include/RTPS/Behavior/StatefulReader.h"
#include "../../../include/RTPS/Behavior/ReaderLocator.h"
#include "../../../include/RTPS/Discovery/SPDPdiscoveredParticipantData.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "../../../include/RTPS/Discovery/SEDPdiscoveredTopicData.h"

#include "../../../include/Utility/Network/TransceiverManager.h"
#include "../../../include/Utility/Timer/DynamicTimer.h"
#include "../../../include/Utility/Network/NetworkManager.h"

#include <stdio.h>
#include <string.h>

namespace CNU_DDS
{
	DomainParticipant::DomainParticipant(DomainId_t a_domain_id, unsigned long count)
	{
		GUID_t				guid;
		Participant*		rtps_part;

#ifdef _WIN32
		*(unsigned int*)guid.guid_prefix.app_id		= GetCurrentProcessId();
		*(unsigned int*)guid.guid_prefix.host_id	= GetCurrentProcessId();
#else
#endif

		domain_id			= a_domain_id;
		status				= new DomainParticipantStatus();
		status_condition	= new StatusCondition();

		status_condition->enabled_statuses	= 0x00;
		status_condition->related_entity	= this;

		listener			= NULL;
		qos					= NULL;

		memcpy(guid.guid_prefix.counter, &count, 4);

		rtps_part	= new Participant(guid.guid_prefix, this);
		this->related_rtps_participant	= rtps_part;

		publisher_list		= new PublisherSeq();
		subscriber_list		= new SubscriberSeq();
		topic_list			= new TopicSeq();
		remote_datawriters	= new DoublyLinkedList<SEDPdiscoveredPublicationData>();
		remote_datareaders	= new DoublyLinkedList<SEDPdiscoveredSubscriptionData>();

		_set_default_publisher_qos();
		_set_default_subscriber_qos();
		_set_default_topic_qos();

		builtin_publisher	= new Publisher();
		builtin_publisher->set_listener(NULL, 0x00000000);
		builtin_publisher->related_participant	= this;
		builtin_publisher->set_qos(default_publisher_qos);

		builtin_subscriber	= new Subscriber();
		builtin_subscriber->set_listener(NULL, 0x00000000);
		builtin_subscriber->related_participant	= this;
		builtin_subscriber->set_qos(default_subscriber_qos);

		_create_builtin_participant_reader((unsigned long)&guid);
		_create_builtin_participant_writer((unsigned long)&guid);
		_create_builtin_publication_reader((unsigned long)&guid);
		_create_builtin_publication_writer((unsigned long)&guid);
		_create_builtin_subscription_reader((unsigned long)&guid);
		_create_builtin_subscription_writer((unsigned long)&guid);
		_create_builtin_topic_reader((unsigned long)&guid);
		_create_builtin_topic_writer((unsigned long)&guid);

		((StatelessWriter*)builtinParticipantsWriter->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, (ByteStream*)_create_PDP_message(), 0);
		pdp_resend_timeout((unsigned long)this);
	}

	DomainParticipant::~DomainParticipant()
	{
		delete(status_condition);
		delete(status);
	}

	ReturnCode_t	DomainParticipant::set_qos(DomainParticipantQos* qos_list)
	{
		ReturnCode_t	result;

		if(qos)	// possible error : RETCODE_IMMUTABLE_POLICY
		{

		}
		else	// possible error : RETCODE_INCONSISTENT_POLICY
		{
			qos		= qos_list;
			result	= RETCODE_OK;
		}

		return result;
	}

	ReturnCode_t	DomainParticipant::get_qos(DomainParticipantQos* qos_list)
	{
		ReturnCode_t	result;

		if(qos)
		{
			qos_list	= (DomainParticipantQos*)qos;
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;	// undefined
		}

		return result;
	}

	ReturnCode_t	DomainParticipant::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result;

		if(listener != NULL && a_listener == NULL)
		{
			delete(listener);
			listener	= NULL;
			status_condition->set_enabled_statuses(mask);
			result		= RETCODE_OK;
		}
		else
		{
			listener	= a_listener;
			status_condition->set_enabled_statuses(mask);
			result		= RETCODE_OK;
		}

		return result;
	}

	Listener*		DomainParticipant::get_listener(void)
	{
		Listener*	result;

		result	= listener;

		return result;
	}

	ReturnCode_t	DomainParticipant::enable()
	{
		ReturnCode_t	result	= RETCODE_OK;

		is_enabled	= true;

		return result;
	}

	pPublisher				DomainParticipant::create_publisher(PublisherQos* qos_list, pPublisherListener a_listener, StatusMask mask)
	{
		Publisher*	result	= NULL;

		if(qos_list == NULL)
		{
			if(get_default_publisher_qos(qos_list) != RETCODE_OK)
			{
				return NULL;
			}
		}

		result	= new Publisher();

		result->set_qos(qos_list);
		result->set_listener((Listener*)a_listener, mask);
		result->related_participant	= this;

		if( (qos != NULL) && ((DomainParticipantQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		publisher_list->insertInRear(result);

		return result;
	}

	pSubscriber				DomainParticipant::create_subscriber(SubscriberQos* qos_list, pSubscriberListener a_listener, StatusMask mask)
	{
		Subscriber*	result	= NULL;

		if(qos_list == NULL)
		{
			if(get_default_subscriber_qos(qos_list) != RETCODE_OK)
			{
				return NULL;
			}
		}

		result	= new Subscriber();
		result->set_qos(qos_list);
		result->set_listener((Listener*)a_listener, mask);
		result->related_participant	= this;

		if( (qos != NULL) && ((DomainParticipantQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		subscriber_list->insertInRear(result);

		return result;
	}

	pTopic					DomainParticipant::create_topic(std::string topic_name, std::string type_name, TopicQos* qos_list, pTopicListener a_listener, StatusMask mask)
	{
		Topic*	result	= NULL;

		if(qos_list == NULL)
		{
			if(get_default_topic_qos(qos_list) != RETCODE_OK)
			{
				return NULL;
			}
		}

		result	= new Topic(topic_name, type_name);

		result->set_qos(qos_list);
		result->related_participant	= this;

		if( (qos != NULL) && ((DomainParticipantQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		topic_list->insertInRear(result);

		return result;
	}

	pContentFilteredTopic	DomainParticipant::create_content_filtered_topic(std::string name, pTopic related_topic, std::string filter_expression, StringSeq* expression_parameteres)
	{
		ContentFilteredTopic*	result	= NULL;

		result	= new ContentFilteredTopic(related_topic, filter_expression);

		if(result && (result->set_expression_parameters(expression_parameteres) != RETCODE_OK))
		{
			delete(result);
			result	= NULL;
		}

		return result;
	}

	pMultiTopic				DomainParticipant::create_multi_topic(std::string name, std::string type_name, std::string subscription_expression, StringSeq* expression_parameters)
	{
		pMultiTopic	result;

		result	= new MultiTopic(name, type_name, subscription_expression);

		if(result && (result->set_expression_parameters(expression_parameters) != RETCODE_OK))
		{
			delete(result);
			result	= NULL;
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_publisher(pPublisher a_publisher)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;
		int				i	= 0;

		Node<Publisher>*	node	= publisher_list->getNodeByIndex(0);	/* getting the head-node of list */

		while(publisher_list->getSize() > i++)
		{
			node	= node->rear;

			if(node->value == a_publisher)
			{
				if(false)	/* The publisher includes DataWriters */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
					break;
				}
				else
				{
					publisher_list->cutNode(node);
					delete(node->value);
					delete(node);
					result	= RETCODE_OK;
					break;
				}
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_subscriber(pSubscriber a_subscriber)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;
		int				i	= 0;

		Node<Subscriber>*	node	= subscriber_list->getNodeByIndex(0);	/* getting the head-node of list */

		while(subscriber_list->getSize() > i++)
		{
			node	= node->rear;

			if(node->value == a_subscriber)
			{
				if(false)	/* The subscriber includes DataReaders */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
					break;
				}
				else
				{
					subscriber_list->cutNode(node);
					delete(node->value);
					delete(node);
					result	= RETCODE_OK;
					break;
				}
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_topic(pTopic a_topic)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;
		int				i	= 0;

		Node<Topic>*	node	= topic_list->getNodeByIndex(0);	/* getting the head-node of list */

		while(topic_list->getSize() > i++)
		{
			node	= node->rear;

			if(node->value == a_topic)
			{
				if(false)	/* The topic is used existing DataReaders, DataWriters, ContentFilteredTopic, or MultiTopic */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
					break;
				}
				else
				{
					topic_list->cutNode(node);
					delete(node->value);
					delete(node);
					result	= RETCODE_OK;
					break;
				}
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_content_filtered_topic(pContentFilteredTopic a_content_filtered_topic)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_multi_topic(pMultiTopic a_multi_topic)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;

		return result;
	}

	ReturnCode_t			DomainParticipant::ignore_participant(InstanceHandle_t handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* register the handle into ignored_handle_list. */


			if(false)	/* if fail registering the handle */
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::ignore_topic(InstanceHandle_t handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* register the handle into ignored_handle_list. */


			if(false)	/* if fail registering the handle */
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::ignore_publication(InstanceHandle_t handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* register the handle into ignored_handle_list. */


			if(false)	/* if fail registering the handle */
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::ignore_subscription(InstanceHandle_t handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* register the handle into ignored_handle_list. */


			if(false)	/* if fail registering the handle */
			{
				result	= RETCODE_OUT_OF_RESOURCES;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::set_default_publisher_qos(PublisherQos* qos_list)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* Checks that resulting policies are self consistent */

			if(false)	/* if there are inconsistent policies */
			{
				result	= RETCODE_INCONSISTENT_POLICY;
			}
			else
			{
				default_publisher_qos	= qos_list;

				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::set_default_subscriber_qos(SubscriberQos* qos_list)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* Checks that resulting policies are self consistent */

			if(false)	/* if there are inconsistent policies */
			{
				result	= RETCODE_INCONSISTENT_POLICY;
			}
			else
			{
				default_subscriber_qos	= qos_list;

				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::set_default_topic_qos(TopicQos* qos_list)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* Checks that resulting policies are self consistent */

			if(false)	/* if there are inconsistent policies */
			{
				result	= RETCODE_INCONSISTENT_POLICY;
			}
			else
			{
				default_topic_qos	= qos_list;

				result				= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_default_publisher_qos(PublisherQos* qos_list)
	{
		ReturnCode_t	result;
		/*
		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{*/
			if(default_publisher_qos == NULL)	/* if the DomainParticipant::set_default_publisher_qos method had never been called */
			{
				/* set the default values listed in the QoS table in Section 7.1.3 DCPS Specification */

				result		= RETCODE_OK;
			}
			else
			{
				qos_list	= default_publisher_qos;

				result		= RETCODE_OK;
			}
		//}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_default_subscriber_qos(SubscriberQos* qos_list)
	{
		ReturnCode_t	result;
		/*
		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{*/
			if(default_subscriber_qos == NULL)	/* if the DomainParticipant::set_default_subscriber_qos method had never been called */
			{
				/* set the default values listed in the QoS table in Section 7.1.3 DCPS Specification */

				result		= RETCODE_OK;
			}
			else
			{
				qos_list	= default_subscriber_qos;

				result		= RETCODE_OK;
			}
		//}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_default_topic_qos(TopicQos* qos_list)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(default_topic_qos == NULL)	/* if the DomainParticipant::set_default_topic_qos method had never been called */
			{
				/* set the default values listed in the QoS table in Section 7.1.3 DCPS Specification */

				result		= RETCODE_OK;
			}
			else
			{
				qos_list	= default_topic_qos;

				result		= RETCODE_OK;
			}
		}

		return result;
	}

	DomainId_t				DomainParticipant::get_domain_id(void)
	{
		DomainId_t	result;

		result	= domain_id;

		return result;
	}

	pPublisher				DomainParticipant::get_builtin_publisher(void)
	{
		return builtin_publisher;
	}

	Subscriber*				DomainParticipant::get_builtin_subscriber(void)
	{
		pSubscriber	result	= NULL;

		result	= builtin_subscriber;

		return result;
	}

	ReturnCode_t			DomainParticipant::get_current_time(Time_t* current_time)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			TIME_TYPE_NATIVE	sys_time	= get_system_time();

			current_time->sec		= sys_time;
			current_time->nanosec	= 0x00000000;

			result	= RETCODE_OK;
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_discovered_participants(InstanceHandleSeq* participant_handles)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(false)	/* if the infrastructure does not maintain the connectivity information */
			{
				result	= RETCODE_UNSUPPORTED;
			}
			else
			{
				result	= RETCODE_OK;
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_discovered_participant_data(ParticipantBuiltinTopicData* participant_data, InstanceHandle_t participant_handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(false)	/* if the infrastructure does not maintain the connectivity information */
			{
				result	= RETCODE_UNSUPPORTED;
			}
			else
			{
				if(false)	/* if operation is fail */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
				}
				else
				{
					result	= RETCODE_OK;
				}
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_discovered_topics(InstanceHandleSeq* topic_handles)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{

		}

		return result;
	}

	ReturnCode_t			DomainParticipant::get_discovered_topic_data(TopicBuiltinTopicData* topic_data, InstanceHandle_t topic_handle)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			if(false)	/* if the infrastructure does not maintain the connectivity information */
			{
				result	= RETCODE_UNSUPPORTED;
			}
			else
			{
				if(false)	/* if operation is fail */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
				}
				else
				{
					result	= RETCODE_OK;
				}
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::delete_contained_entities(void)
	{
		ReturnCode_t		result;
		DomainEntity*		domain_entity;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			while(publisher_list->getSize() > 0)
			{
				domain_entity	= publisher_list->popFirst();

				result	= ((Publisher*)domain_entity)->delete_contained_entities();

				if(result != RETCODE_OK)
				{
					delete((Publisher*)domain_entity);
				}
				else
				{
					return result;
				}
			}

			while(subscriber_list->getSize() > 0)
			{
				domain_entity	= subscriber_list->popFirst();

				result	= ((Subscriber*)domain_entity)->delete_contained_entities();

				if(result != RETCODE_OK)
				{
					delete((Subscriber*)domain_entity);
				}
				else
				{
					return result;
				}
			}

			while(topic_list->getSize() > 0)
			{
				domain_entity	= topic_list->popFirst();
				delete((Topic*)domain_entity);
			}
		}

		return result;
	}

	ReturnCode_t			DomainParticipant::assert_liveliness(void)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			/* which thing sends liveliness messages(?) */
		}

		return result;
	}

	pTopic					DomainParticipant::find_topic(std::string topic_name, Duration_t timeout)
	{
		Topic*					result				= NULL;
		TopicLookupHandle*		topic_lookup_handle	= NULL;
		int						i					= 0;
		MUTEX_TYPE_NATIVE		mutex;
		CONDITION_TYPE_NATIVE	cond;


		Node<Topic>*	node	= topic_list->getNodeByIndex(0);	// getting the head-node of list

		while(topic_list->getSize() > i++)
		{
			node	= node->rear;

			result	= node->value;

			if(result && (result->get_topic_name().compare(topic_name) == 0))	// success
			{
				return result;
			}

			result	= NULL;
		}

		if(result == NULL)
		{
			if((timeout.sec == 0) && (timeout.nanosec == 0))
			{
				return result;	// must be NULL
			}
			else
			{
				create_mutex(&mutex);
				create_condition(&cond);

				/* register the topic and the condition information into DomainParticipant */
				topic_lookup_handle	= new TopicLookupHandle();
				topic_lookup_handle->topic_name	= topic_name;
				topic_lookup_handle->condition	= &cond;

				topic_find_list.insertInRear(topic_lookup_handle);

				/* register a timer about deadline */


				mutex_lock(&mutex);
				condition_wait(&cond, &mutex);
				mutex_unlock(&mutex);

				timeout.sec		= 0;
				timeout.nanosec	= 0;

				return	find_topic(topic_name, timeout);
			}
		}

		return result;
	}

	pTopicDescription		DomainParticipant::lookup_topic_description(std::string name)
	{
		pTopicDescription	result	= NULL;
		int					i		= 0;

		Node<Topic>*	node	= topic_list->getNodeByIndex(0);	// getting the head-node of list

		while(topic_list->getSize() > i++)
		{
			node	= node->rear;

			result	= node->value->get_topic_description();

			if(result && (result->get_topic_name().compare(name) == 0))	// success
			{
				return result;
			}

			result	= NULL;
		}

		return result;
	}

	bool				DomainParticipant::contains_entity(InstanceHandle_t a_handle)
	{
		bool			result;
		int				i		= 0;
		Node<Entity>*	node	= NULL;


		/* Checks topics created by this participant */

		node	= (Node<Entity>*)topic_list->getNodeByIndex(0);	/* getting the head-node of list */
		i		= 0;
		result	= false;

		while(topic_list->getSize() > i++)
		{
			node	= node->rear;

			if((InstanceHandle_t)node->value == a_handle)
			{
				result	= true;

				return	result;
			}
		}


		/* Checks publishers created by this participant */

		node	= (Node<Entity>*)publisher_list->getNodeByIndex(0);	/* getting the head-node of list */
		i		= 0;
		result	= false;

		while(publisher_list->getSize() > i++)
		{
			node	= node->rear;

			if((InstanceHandle_t)node->value == a_handle)
			{
				result	= true;

				return	result;
			}
		}


		/* Checks subscribers created by this participant */

		node	= (Node<Entity>*)subscriber_list->getNodeByIndex(0);	/* getting the head-node of list */
		i		= 0;
		result	= false;

		while(subscriber_list->getSize() > i++)
		{
			node	= node->rear;

			if((InstanceHandle_t)node->value == a_handle)
			{
				result	= true;

				return	result;
			}
		}


		/* Checks built-in publisher&subscriber created by this participant */

		if((builtin_publisher != NULL) && ((InstanceHandle_t)builtin_publisher == a_handle))
		{
			result	= true;
		}
		else if((builtin_subscriber != NULL) && ((InstanceHandle_t)builtin_subscriber == a_handle))
		{
			result	= true;
		}
		else
		{
			result	= false;
		}

		return result;
	}

	void				DomainParticipant::topic_find_timeout(void* arg)
	{
		TopicLookupHandle*	handle	= (TopicLookupHandle*)arg;

		condition_signal(handle->condition);

		delete(handle);
	}

	void				DomainParticipant::pdp_resend_timeout(unsigned long arg)
	{
		DomainParticipant*	part	= (DomainParticipant*)arg;
		DynamicTimer*		timer	= DynamicTimer::getDynamicTimerInstance();
		TimeUnit*			unit;

		((StatelessWriter*)part->builtinParticipantsWriter->related_rtps_writer)->unsent_changes_reset();

		unit			= new TimeUnit(3000);
		unit->setDataValue(arg);
		unit->setExecuteFunction(DomainParticipant::pdp_resend_timeout);

		timer->regist(unit);
	}

	void					DomainParticipant::_send_a_pdp_message()
	{
		((StatelessWriter*)this->builtinParticipantsWriter->related_rtps_writer)->unsent_changes_reset();
	}

	void					DomainParticipant::_datawriter_is_created(pDataWriter a_data_writer)
	{
		ByteStream*	stream;
		TransceiverManager*	tm	= TransceiverManager::getManagerInstance();
		Transceiver*		tr	= tm->getTransceiverInstance(TRANSCEIVER_ID_DEFAULT_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));

		tr->regist(a_data_writer->related_rtps_writer);

		stream	= (ByteStream*)_create_Pub_EDP_message(a_data_writer);

		((StatefulWriter*)builtinPublicationsWriter->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, stream, 0);

		int								i;
		ReaderProxy*					proxy;
		SEDPdiscoveredSubscriptionData*	remote_reader;
		Locator_t*						locator;

		for(i=1; i<=remote_datareaders->getSize(); i++)
		{
			remote_reader	= remote_datareaders->getNodeByIndex(i)->value;

			if(remote_reader->topic_name.compare(a_data_writer->related_topic->get_topic_name()) == 0)
			{

				locator	= new Locator_t;
				memcpy(locator, remote_reader->unicast_locator_list->getFirst(), sizeof(Locator_t));

				proxy	= new ReaderProxy((unsigned long)a_data_writer->related_rtps_writer);
				proxy->remote_reader_guid	= remote_reader->remote_reader_guid;
				proxy->unicast_locator_list->insertInRear(locator);
				proxy->start();

				((StatefulWriter*)a_data_writer->related_rtps_writer)->matched_reader_add(proxy);
			}
		}
	}

	void					DomainParticipant::_datareader_is_created(pDataReader a_data_reader)
	{
		ByteStream*	stream;
		TransceiverManager*	tm	= TransceiverManager::getManagerInstance();
		Transceiver*		tr	= tm->getTransceiverInstance(TRANSCEIVER_ID_DEFAULT_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));

		tr->regist(a_data_reader->related_rtps_reader);

		stream	= (ByteStream*)_create_Sub_EDP_message(a_data_reader);

		((StatefulWriter*)builtinSubscriptionsWriter->related_rtps_writer)->new_change(CHANGE_KIND_ALIVE, stream, 0);

		int								i;
		WriterProxy*					proxy;
		SEDPdiscoveredPublicationData*	remote_writer;
		Locator_t*						locator;

		for(i=1; i<=remote_datawriters->getSize(); i++)
		{
			remote_writer	= remote_datawriters->getNodeByIndex(i)->value;

			if(remote_writer->topic_name.compare(a_data_reader->related_topic_description->get_topic_name()) == 0)
			{

				locator	= new Locator_t;
				memcpy(locator, remote_writer->unicast_locator_list->getFirst(), sizeof(Locator_t));

				proxy	= new WriterProxy((unsigned long)a_data_reader->related_rtps_reader);
				proxy->remote_writer_guid	= remote_writer->remote_writer_guid;
				proxy->unicast_locator_list->insertInRear(locator);
				proxy->start();

				((StatefulReader*)a_data_reader->related_rtps_reader)->matched_writer_add(proxy);
			}
		}
	}

	void					DomainParticipant::_remote_datawriter_is_discovered(SEDPdiscoveredPublicationData* remote_writer)
	{
		int	i, j;
		Subscriber*		sub;
		DataReader*		reader;
		WriterProxy*	proxy;
		Locator_t*		locator;

		//printf("DomainParticipant::_remote_dw_is_discovered()\n");

		this->remote_datawriters->insertInRear(remote_writer);

		for(i=1; i<=subscriber_list->getSize(); i++)
		{
			sub	= subscriber_list->getNodeByIndex(i)->value;

			for(j=1; j<=sub->datareader_list->getSize(); j++)
			{
				reader	= sub->datareader_list->getNodeByIndex(j)->value;

				if(reader->related_topic_description->get_topic_name().compare(remote_writer->topic_name) == 0)
				{
					locator	= new Locator_t;
					memcpy(locator, remote_writer->unicast_locator_list->getFirst(), sizeof(Locator_t));

					proxy	= new WriterProxy((unsigned long)reader->related_rtps_reader);
					proxy->remote_writer_guid	= remote_writer->remote_writer_guid;
					proxy->unicast_locator_list->insertInRear(locator);
					proxy->start();

					((StatefulReader*)reader->related_rtps_reader)->matched_writer_add(proxy);
				}
			}
		}
	}

	void					DomainParticipant::_remote_datareader_is_discovered(SEDPdiscoveredSubscriptionData* remote_reader)
	{
		int	i, j;
		Publisher*		pub;
		DataWriter*		writer;
		ReaderProxy*	proxy;
		Locator_t*		locator;

		//printf("DomainParticipant::_remote_dr_is_discovered()\n");

		this->remote_datareaders->insertInRear(remote_reader);

		for(i=1; i<=publisher_list->getSize(); i++)
		{
			pub	= publisher_list->getNodeByIndex(i)->value;

			for(j=1; j<=pub->datawriter_list->getSize(); j++)
			{
				writer	= pub->datawriter_list->getNodeByIndex(j)->value;

				if(writer->related_topic->get_topic_name().compare(remote_reader->topic_name) == 0)
				{
					locator	= new Locator_t;
					memcpy(locator, remote_reader->unicast_locator_list->getFirst(), sizeof(Locator_t));

					proxy	= new ReaderProxy((unsigned long)writer->related_rtps_writer);
					proxy->remote_reader_guid	= remote_reader->remote_reader_guid;
					proxy->unicast_locator_list->insertInRear(locator);
					proxy->start();

					((StatefulWriter*)writer->related_rtps_writer)->matched_reader_add(proxy);
				}
			}
		}
	}

	unsigned long					DomainParticipant::_create_PDP_message()
	{
		SPDPdiscoveredParticipantData*	data		= new SPDPdiscoveredParticipantData();
		ByteStream*						stream;
		NetworkInfoManager*				net_man		= NetworkInfoManager::getNetworkInfoManagerInstance();
		Locator_t*						locator;
		TransceiverManager*				tm			= TransceiverManager::getManagerInstance();
		Transceiver*					transceiver	= NULL;

		//data->available_builtin_endpoints		= 0x3f;
		data->available_builtin_endpoints		= 0x3C;
		data->expects_inline_qos				= false;
		data->guid_prefix						= this->related_rtps_participant->guid.guid_prefix;
		data->lease_duration					= Duration_t(100, 0);
		//data->manual_liveliness_count			= ((DomainParticipantStatus*)this->status)->
		data->manual_liveliness_count.value		= 0x00;
		data->protocol_version					= PROTOCOL_VERSION;
		data->vendor_id							= VENDORID_UNKNOWN;

		locator									= new Locator_t();
		transceiver								= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		*locator								= transceiver->locator;
		data->metatraffic_multicast_locator_list	= new LocatorSeq();
		data->metatraffic_multicast_locator_list->insertInRear(locator);

		locator									= new Locator_t();
		transceiver								= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		*locator								= transceiver->locator;
		data->metatraffic_unicast_locator_list	= new LocatorSeq();
		data->metatraffic_unicast_locator_list->insertInRear(locator);

		locator									= new Locator_t();
		transceiver								= tm->getTransceiverInstance(TRANSCEIVER_ID_DEFAULT_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		*locator								= transceiver->locator;
		data->default_unicast_locator_list		= new LocatorSeq();
		data->default_unicast_locator_list->insertInRear(locator);

		data->default_multicast_locator_list	= new LocatorSeq();

		stream	= data->Serialize();
		delete(data);

		return (unsigned long)stream;
	}


	unsigned long			DomainParticipant::_create_Pub_EDP_message(pDataWriter a_data_writer)
	{
		SEDPdiscoveredPublicationData*	data		= new SEDPdiscoveredPublicationData();
		ByteStream*						stream		= NULL;
		DataWriterQos*					writer_qos	= new DataWriterQos();
		Locator_t*						locator;
		Publisher*						pub			= a_data_writer->get_publisher();
		PublisherQos*					pub_qos		= new PublisherQos();

		a_data_writer->get_qos(writer_qos);

		data->topic_name			= a_data_writer->get_topic()->get_topic_name();
		data->type_name				= a_data_writer->get_topic()->get_type_name();
		data->remote_writer_guid	= a_data_writer->related_rtps_writer->guid;

		memcpy(&data->deadline, &writer_qos->deadline, sizeof(DeadlineQosPolicy));
		memcpy(&data->destination_order, &writer_qos->destination_order, sizeof(DestinationOrderQosPolicy));
		memcpy(&data->durability, &writer_qos->durabiltiy, sizeof(DurabilityQosPolicy));
		memcpy(&data->durability_service, &writer_qos->durability_service, sizeof(DurabilityServiceQosPolicy));
		memcpy(&data->latency_budget, &writer_qos->latency_budget, sizeof(LatencyBudgetQosPolicy));
		memcpy(&data->lifespan, &writer_qos->lifespan, sizeof(LifespanQosPolicy));
		memcpy(&data->liveliness, &writer_qos->liveliness,sizeof(LivelinessQosPolicy));
		memcpy(&data->ownership, &writer_qos->ownership, sizeof(OwnershipQosPolicy));
		memcpy(&data->ownership_strength, &writer_qos->ownership_strength, sizeof(OwnershipStrengthQosPolicy));
		memcpy(&data->reliability, &writer_qos->reliability, sizeof(ReliabilityQosPolicy));

		pub->get_qos(pub_qos);
		memcpy(&data->presentation, &pub_qos->presentation, sizeof(PresentationQosPolicy));
		//memcpy(&data->partition, &pub_qos->partition, sizeof(PartitionQosPolicy));
		memcpy(&data->group_data, &pub_qos->group_data, sizeof(GroupDataQosPolicy));

		locator							= new Locator_t();
		memcpy(locator, related_rtps_participant->default_unicast_locator_list->getFirst(), sizeof(Locator_t));
		data->unicast_locator_list->insertInRear(locator);

		stream	= data->Serialize();

		delete(pub_qos);
		delete(writer_qos);
		delete(data);

		return (unsigned long)stream;
	}

	unsigned long			DomainParticipant::_create_Sub_EDP_message(pDataReader a_data_reader)
	{
		SEDPdiscoveredSubscriptionData*	data		= new SEDPdiscoveredSubscriptionData();
		ByteStream*						stream		= NULL;
		DataReaderQos*					reader_qos	= new DataReaderQos();
		Locator_t*						locator		= NULL;
		Subscriber*						sub			= a_data_reader->get_subscriber();
		SubscriberQos*					sub_qos		= new SubscriberQos();

		a_data_reader->get_qos(reader_qos);

		data->topic_name			= a_data_reader->get_topic_description()->get_topic_name();
		data->type_name				= a_data_reader->get_topic_description()->get_type_name();
		data->remote_reader_guid	= a_data_reader->related_rtps_reader->guid;

		memcpy(&data->durability, &reader_qos->durability, sizeof(DurabilityQosPolicy));
		memcpy(&data->deadline, &reader_qos->deadline, sizeof(DeadlineQosPolicy));
		memcpy(&data->latency_budget, &reader_qos->latency_budget, sizeof(LatencyBudgetQosPolicy));
		memcpy(&data->liveliness, &reader_qos->liveliness, sizeof(LivelinessQosPolicy));
		memcpy(&data->reliability, &reader_qos->reliability, sizeof(ReliabilityQosPolicy));
		memcpy(&data->destination_order, &reader_qos->destination_order, sizeof(DestinationOrderQosPolicy));
		memcpy(&data->user_data, &reader_qos->user_data, sizeof(UserDataQosPolicy));
		memcpy(&data->ownership, &reader_qos->ownership, sizeof(OwnershipQosPolicy));
		memcpy(&data->time_based_filter, &reader_qos->time_based_filter, sizeof(TimeBasedFilterQosPolicy));

		sub->get_qos(sub_qos);
		memcpy(&data->presentation, &sub_qos->presentation, sizeof(PresentationQosPolicy));
		//memcpy(&data->partition, &sub_qos->partition, sizeof(PartitionQosPolicy));
		memcpy(&data->group_data, &sub_qos->group_data, sizeof(GroupDataQosPolicy));

		locator							= new Locator_t();
		memcpy(locator, related_rtps_participant->default_unicast_locator_list->getFirst(), sizeof(Locator_t));
		data->unicast_locator_list->insertInRear(locator);


		stream	= data->Serialize();

		delete(sub_qos);
		delete(reader_qos);
		delete(data);

		return (unsigned long)stream;
	}

	void					DomainParticipant::_create_builtin_participant_writer(unsigned long p_guid)
	{
		BuiltinParticipantsWriterListener*	part_writer_listener	= new BuiltinParticipantsWriterListener();
		DataWriter*							writer;
		StatelessWriter*					rtps_writer;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		Locator_t							locator	= {LOCATOR_KIND_UDPv4, related_rtps_participant->PORT_BASE_NUMBER + ( related_rtps_participant->DOMAIN_ID_GAIN * domain_id ) + related_rtps_participant->ADDITIONAL_OFFSET_D0, {0,0,0,0,0,0,0,0,0,0,0,0,239,255,0,1 }/*"239.255.0.1"*/};

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SPDP_BUILTIN_PARTICIPANT_WRITER;

		writer		= new DataWriter();
		builtinParticipantsWriter	= writer;
		rtps_writer	= new StatelessWriter(guid, RELIABILITY_KIND_BEST_EFFORT, Duration_t(1, 0));

		writer->set_listener(part_writer_listener, 0xFFFFFFFF);
		writer->related_topic				= new Topic("BuiltinParticipantsDiscovery", "DiscoveredParticiapantData");
		writer->related_participant			= this;
		writer->related_publisher			= builtin_publisher;
		writer->related_rtps_writer			= rtps_writer;
		this->builtin_publisher->add_datawriter(writer);

		rtps_writer							= (StatelessWriter*)writer->related_rtps_writer;
		rtps_writer->related_dcps_writer	= writer;
		rtps_writer->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);

		rtps_writer->reader_locator_add(locator);
	}

	void					DomainParticipant::_create_builtin_participant_reader(unsigned long p_guid)
	{
		BuiltinParticipantsReaderListener*	part_reader_listener	= new BuiltinParticipantsReaderListener();
		DataReader*							reader;
		StatelessReader*					rtps_reader;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SPDP_BUILTIN_PARTICIPANT_READER;

		reader		= new DataReader();
		builtinParticipantsReader	= reader;
		rtps_reader	= new StatelessReader(guid, RELIABILITY_KIND_BEST_EFFORT);

		reader->set_listener(part_reader_listener, 0xFFFFFFFF);
		reader->related_topic_description	= new TopicDescription("BuiltinParticipantsDiscovery", "DiscoveredParticipantData");
		reader->related_participant			= this;
		reader->related_subscriber			= builtin_subscriber;
		reader->related_rtps_reader			= rtps_reader;
		this->builtin_subscriber->add_datareader(reader);

		rtps_reader							= (StatelessReader*)reader->related_rtps_reader;
		rtps_reader->related_dcps_reader	= reader;
		rtps_reader->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);
	}

	void					DomainParticipant::_create_builtin_publication_writer(unsigned long p_guid)
	{
		BuiltinPublicationsWriterListener*	pub_writer_listener	= new BuiltinPublicationsWriterListener();
		DataWriter*							writer;
		DataWriterQos*						writer_qos;
		StatefulWriter*						rtps_writer;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_WRITER;

		writer		= new DataWriter();
		builtinPublicationsWriter	= writer;
		rtps_writer	= new StatefulWriter(guid, RELIABILITY_KIND_RELIABLE, true);

		writer->set_listener(pub_writer_listener, 0xFFFFFFFF);
		writer->related_topic				= new Topic("BuiltinPublicationsDiscovery", "DiscoveredWriterData");
		writer->related_participant			= this;
		writer->related_publisher			= builtin_publisher;
		writer->related_rtps_writer			= rtps_writer;
		this->builtin_publisher->add_datawriter(writer);

		writer_qos	= new DataWriterQos();
		this->builtin_publisher->get_default_datawriter_qos(writer_qos);
		writer->set_qos(writer_qos);
		delete(writer_qos);
		//writer_qos

		rtps_writer							= (StatefulWriter*)writer->related_rtps_writer;
		rtps_writer->related_dcps_writer	= writer;
		rtps_writer->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);
	}

	void					DomainParticipant::_create_builtin_publication_reader(unsigned long p_guid)
	{
		BuiltinPublicationsReaderListener*	pub_reader_listener	= new BuiltinPublicationsReaderListener();
		DataReader*							reader;
		DataReaderQos*						reader_qos;
		StatefulReader*						rtps_reader;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SEDP_BUILTIN_PUBLICATIONS_READER;

		reader		= new DataReader();
		builtinPublicationsReader	= reader;
		rtps_reader	= new StatefulReader(guid, RELIABILITY_KIND_RELIABLE);

		reader->set_listener(pub_reader_listener, 0xFFFFFFFF);
		reader->related_topic_description	= new TopicDescription("BuiltinPublicationsDiscovery", "DiscoveredWriterData");
		reader->related_participant			= this;
		reader->related_subscriber			= builtin_subscriber;
		reader->related_rtps_reader			= rtps_reader;
		this->builtin_subscriber->add_datareader(reader);

		reader_qos	= new DataReaderQos();
		this->builtin_subscriber->get_default_datareader_qos(reader_qos);
		reader->set_qos(reader_qos);
		delete(reader_qos);
		//reader_qos

		rtps_reader							= (StatefulReader*)reader->related_rtps_reader;
		rtps_reader->related_dcps_reader	= reader;
		rtps_reader->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);
	}

	void					DomainParticipant::_create_builtin_subscription_writer(unsigned long p_guid)
	{
		BuiltinSubscriptionsWriterListener*	sub_writer_listener	= new BuiltinSubscriptionsWriterListener();
		DataWriter*							writer;
		DataWriterQos*						writer_qos;
		StatefulWriter*						rtps_writer;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_WRITER;

		writer		= new DataWriter();
		builtinSubscriptionsWriter	= writer;
		rtps_writer	= new StatefulWriter(guid, RELIABILITY_KIND_RELIABLE, true);

		writer->set_listener(sub_writer_listener, 0xFFFFFFFF);
		writer->related_topic				= new Topic("BuiltinSubscriptionsDiscovery", "DiscoveredReaderData");
		writer->related_participant			= this;
		writer->related_publisher			= builtin_publisher;
		writer->related_rtps_writer			= rtps_writer;
		this->builtin_publisher->add_datawriter(writer);

		writer_qos	= new DataWriterQos();
		this->builtin_publisher->get_default_datawriter_qos(writer_qos);
		writer->set_qos(writer_qos);
		delete(writer_qos);
		//writer_qos

		rtps_writer							= (StatefulWriter*)writer->related_rtps_writer;
		rtps_writer->related_dcps_writer	= writer;
		rtps_writer->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_writer);
	}

	void					DomainParticipant::_create_builtin_subscription_reader(unsigned long p_guid)
	{
		BuiltinSubscriptionsReaderListener*	sub_reader_listener	= new BuiltinSubscriptionsReaderListener();
		DataReader*							reader;
		DataReaderQos*						reader_qos;
		StatefulReader*						rtps_reader;

		TransceiverManager*					tm	= TransceiverManager::getManagerInstance();
		Transceiver*						transceiver;

		GUID_t								guid;

		guid.guid_prefix	= ((GUID_t*)p_guid)->guid_prefix;
		guid.entity_id		= ENTITYID_SEDP_BUILTIN_SUBSCRIPTIONS_READER;

		reader		= new DataReader();
		builtinSubscriptionsReader	= reader;
		rtps_reader	= new StatefulReader(guid, RELIABILITY_KIND_RELIABLE);

		reader->set_listener(sub_reader_listener, 0xFFFFFFFF);
		reader->related_topic_description	= new TopicDescription("BuiltinSubscriptionsDiscovery", "DiscoveredReaderData");
		reader->related_participant			= this;
		reader->related_subscriber			= builtin_subscriber;
		reader->related_rtps_reader			= rtps_reader;
		this->builtin_subscriber->add_datareader(reader);

		reader_qos	= new DataReaderQos();
		this->builtin_subscriber->get_default_datareader_qos(reader_qos);
		reader->set_qos(reader_qos);
		delete(reader_qos);
		//reader_qos

		rtps_reader							= (StatefulReader*)reader->related_rtps_reader;
		rtps_reader->related_dcps_reader	= reader;
		rtps_reader->start();

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_MULTICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);

		transceiver	= tm->getTransceiverInstance(TRANSCEIVER_ID_METATRAFFIC_UNICAST + (domain_id * TRANSCEIVER_ID_DOMAIN_ID_GAIN));
		transceiver->regist(rtps_reader);
	}

	void					DomainParticipant::_create_builtin_topic_writer(unsigned long p_guid)
	{

	}

	void					DomainParticipant::_create_builtin_topic_reader(unsigned long p_guid)
	{

	}

	void					DomainParticipant::_set_default_publisher_qos()
	{
		this->default_publisher_qos		= new PublisherQos();
		this->default_publisher_qos->entity_factory.auto_enable_created_entities	= false;
		this->default_publisher_qos->presentation.access_scope		= INSTANCE_PRESENTATION_QOS;
		this->default_publisher_qos->presentation.coherent_access	= false;
		this->default_publisher_qos->presentation.ordered_access	= true;
	}

	void					DomainParticipant::_set_default_subscriber_qos()
	{
		this->default_subscriber_qos	= new SubscriberQos();
		this->default_subscriber_qos->entity_factory.auto_enable_created_entities	= false;
		this->default_subscriber_qos->presentation.access_scope		= INSTANCE_PRESENTATION_QOS;
		this->default_subscriber_qos->presentation.coherent_access	= false;
		this->default_subscriber_qos->presentation.ordered_access	= true;
	}

	void					DomainParticipant::_set_default_topic_qos()
	{
		this->default_topic_qos			= new TopicQos();
	}
}
