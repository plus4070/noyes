#include "../../../include/DCPS/Publication/Publisher.h"
#include "../../../include/DCPS/Publication/DataWriter.h"
#include "../../../include/DCPS/Publication/DataWriterListener.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"
#include "../../../include/Status/EntityStatus/PublisherStatus.h"
#include "../../../include/Status/Condition/StatusCondition.h"

#include "../../../include/RTPS/Structure/Participant.h"
#include "../../../include/RTPS/Behavior/StatefulWriter.h"

#include <stdio.h>

namespace CNU_DDS
{
	Publisher::Publisher()
	{
		status				= new PublisherStatus();
		status_condition	= new StatusCondition();
		datawriter_list		= new DataWriterSeq();

		status_condition->enabled_statuses	= 0x00;
		status_condition->related_entity	= this;

		is_enabled			= false;
		listener			= NULL;
		qos					= NULL;

		_set_default_datawriter_qos();

		entity_key			= 0;
	}

	Publisher::~Publisher()
	{
		delete(status_condition);
		delete(status);
		delete(datawriter_list);
	}

	ReturnCode_t		Publisher::get_qos(PublisherQos* qos_list)
	{
		ReturnCode_t	result;

		if(qos_list == NULL)
		{
			result	= RETCODE_BAD_PARAMETER;
		}
		else if(qos)
		{
			memcpy(qos_list, qos, sizeof(PublisherQos));
			result		= RETCODE_OK;
		}
		else
		{
			result		= RETCODE_PRECONDITION_NOT_MET;	// undefined
		}

		return	result;
	}

	ReturnCode_t		Publisher::set_qos(PublisherQos* qos_list)
	{
		ReturnCode_t	result;

		if(!is_enabled && qos == NULL)		// possible error : RETCODE_INCONSISTENT_POLICY
		{
			qos	= new PublisherQos();

			((PublisherQos*)qos)->entity_factory.auto_enable_created_entities	= qos_list->entity_factory.auto_enable_created_entities;
			memcpy(((PublisherQos*)qos)->group_data.value, qos_list->group_data.value, MAX_SIZE_OF_GROUP_DATA_QOS_POLICY);
			((PublisherQos*)qos)->presentation.access_scope		= qos_list->presentation.access_scope;
			((PublisherQos*)qos)->presentation.coherent_access	= qos_list->presentation.coherent_access;
			((PublisherQos*)qos)->presentation.ordered_access	= qos_list->presentation.ordered_access;
			result	= RETCODE_OK;
			return result;
		}
		/*
		else	// possible error : RETCODE_IMMUTABLE_POLICY
		{
			
		}*/

		return result;
	}

	Listener*			Publisher::get_listener(void)
	{
		Listener*	result;

		result	= listener;

		return result;
	}

	ReturnCode_t		Publisher::set_listener(Listener* a_listener, StatusMask mask)
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

	ReturnCode_t		Publisher::enable(void)
	{
		ReturnCode_t	result	= RETCODE_OK;

		is_enabled	= true;

		return	result;
	}

	pDataWriter			Publisher::create_datawriter(pTopic a_topic, DataWriterQos* writer_qos, DataWriterListener* a_listener, StatusMask mask)
	{
		DataWriter*			result;
		Participant*		rtps_part	= related_participant->related_rtps_participant;
		StatefulWriter*		rtps_writer;
		GUID_t				guid	= {0, };

		if(writer_qos == NULL)
		{
			writer_qos	= new DataWriterQos();
			if(get_default_datawriter_qos(writer_qos) != RETCODE_OK)
			{
				return NULL;
			}
		}


		result	= new DataWriter();

		result->set_qos(writer_qos);
		result->set_listener(a_listener, mask);
		result->related_publisher	= this;
		result->related_participant	= related_participant;
		result->related_topic		= a_topic;

		if( (qos != NULL) && ((PublisherQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		datawriter_list->insertInRear(result);

		guid.guid_prefix					= related_participant->related_rtps_participant->guid.guid_prefix;
		guid.entity_id.entity_key[0]		= ++entity_key;
		guid.entity_id.entity_kind			= ENTITY_KIND_USER_DEFINED_WRITER_WITH_KEY;
		rtps_writer							= new StatefulWriter(guid, RELIABILITY_KIND_RELIABLE, true);
		rtps_writer->related_dcps_writer	= result;
		result->related_rtps_writer			= (Writer*)rtps_writer;

		related_participant->_datawriter_is_created(result);
		rtps_writer->start();

		return	result;
	}

	void				Publisher::add_datawriter(pDataWriter writer)
	{
		datawriter_list->insertInRear(writer);
	}

	ReturnCode_t		Publisher::delete_datawriter(pDataWriter a_datawriter)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;
		int				i		= 0;

		Node<DataWriter>*	node	= datawriter_list->getNodeByIndex(0);	/* getting the head-node of list */

		while(datawriter_list->getSize() > i++)
		{
			node	= node->rear;

			if(node->value == a_datawriter)
			{
				if(false)	/* The publisher includes DataWriters */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
					break;
				}
				else
				{
					datawriter_list->cutNode(node);
					delete(node->value);
					delete(node);
					result	= RETCODE_OK;
					break;
				}
			}
		}

		return	result;
	}

	pDataWriter			Publisher::lookup_datawriter(std::string topic_name)
	{
		DataWriter*	result	= NULL;
		int			i		= 0;

		for(i=1; i<=datawriter_list->getSize(); i++)
		{
			result	= datawriter_list->getNodeByIndex(i)->value;

			if(result && (result->get_topic()->get_topic_name().compare(topic_name) == 0))	// success
			{
				return result;
			}

			result	= NULL;
		}

		return	result;
	}

	ReturnCode_t		Publisher::suspend_publications(void)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		Publisher::resume_publications(void)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else if(false /*suspend_publications was not called*/)
		{
			result	= RETCODE_PRECONDITION_NOT_MET;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		Publisher::begin_coherent_changes(void)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		Publisher::end_coherent_changes(void)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else if(false /*begin_coherent_changes was not called*/)
		{
			result	= RETCODE_PRECONDITION_NOT_MET;
		}
		else
		{

		}

		return	result;
	}

	ReturnCode_t		Publisher::wait_for_acknowledgements(Duration_t max_wait)
	{
		ReturnCode_t	result;
		Duration_t		remains;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			DataWriter*	datawriter	= NULL;
			int			i			= 0;

			Node<DataWriter>*	node	= datawriter_list->getNodeByIndex(0);	// getting the head-node of list

			while(datawriter_list->getSize() > i++)
			{
				node	= node->rear;

				datawriter	= node->value;

				if(datawriter)	// success
				{
					result	= datawriter->wait_for_acknowledgements(remains);
				}

				/* calculates remain wait duration */

				if(i < datawriter_list->getSize() && remains.sec <= 0 && remains.nanosec <= 0)
				{
					result	= RETCODE_TIMEOUT;
					break;
				}
				else
				{
					result	= RETCODE_OK;
				}
			}
		}

		return	result;
	}

	pDomainParticipant	Publisher::get_participant(void)
	{
		DomainParticipant*	result;

		result	= related_participant;

		return	result;
	}

	ReturnCode_t		Publisher::delete_contained_entities(void)
	{
		DataWriter*			datawriter;
		ReturnCode_t		result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			while(datawriter_list->getSize() > 0)
			{
				datawriter	= datawriter_list->popFirst();

				if(datawriter && true /* need to check whether can delete or not */)
				{
					delete(datawriter);
				}
			}

			result	= RETCODE_OK;
		}

		return result;
	}

	ReturnCode_t		Publisher::set_default_datawriter_qos(DataWriterQos* qos_list)
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
				default_datawriter_qos	= qos_list;

				result	= RETCODE_OK;
			}
		}

		return	result;
	}

	ReturnCode_t		Publisher::get_default_datawriter_qos(DataWriterQos* qos_list)
	{
		ReturnCode_t	result;

		memcpy(qos_list, default_datawriter_qos, sizeof(DataWriterQos));

		result		= RETCODE_OK;

		return	result;
	}

	ReturnCode_t		Publisher::copy_from_topic_qos(DataWriterQos* a_datawriter_qos, TopicQos* a_topic_qos)
	{
		ReturnCode_t	result;

		if(!is_enabled)
		{
			result	= RETCODE_NOT_ENABLED;
		}
		else
		{
			a_datawriter_qos->deadline				= a_topic_qos->deadline;
			a_datawriter_qos->destination_order		= a_topic_qos->destination_order;
			a_datawriter_qos->durability_service	= a_topic_qos->durability_service;
			a_datawriter_qos->durabiltiy			= a_topic_qos->durability;
			a_datawriter_qos->history				= a_topic_qos->history;
			a_datawriter_qos->latency_budget		= a_topic_qos->latency_budget;
			a_datawriter_qos->lifespan				= a_topic_qos->lifespan;
			a_datawriter_qos->liveliness			= a_topic_qos->liveliness;
			a_datawriter_qos->ownership				= a_topic_qos->ownership;
			a_datawriter_qos->reliability			= a_topic_qos->reliability;
			a_datawriter_qos->resource_limits		= a_topic_qos->resource_limits;
			a_datawriter_qos->transport_priority	= a_topic_qos->transport_priority;

			result	= RETCODE_OK;
		}

		return	result;
	}

	void				Publisher::_set_default_datawriter_qos(void)
	{
		this->default_datawriter_qos	= new DataWriterQos();

		this->default_datawriter_qos->deadline.period.sec		= 0;
		this->default_datawriter_qos->deadline.period.nanosec	= 0;

		this->default_datawriter_qos->destination_order.kind	= BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS;

		this->default_datawriter_qos->durability_service.history_kind	= KEEP_ALL_HISTORY_QOS;
		this->default_datawriter_qos->durability_service.history_depth	= -1;
		this->default_datawriter_qos->durability_service.max_instances	= -1;
		this->default_datawriter_qos->durability_service.max_samples	= -1;
		this->default_datawriter_qos->durability_service.max_samples_per_instance	= -1;

		this->default_datawriter_qos->durabiltiy.kind	= VOLATILE_DURABILITY_QOS;
		
		this->default_datawriter_qos->history;
		this->default_datawriter_qos->history.kind	= KEEP_ALL_HISTORY_QOS;
		this->default_datawriter_qos->history.depth	= -1;

		this->default_datawriter_qos->latency_budget.duration.sec		= 0;
		this->default_datawriter_qos->latency_budget.duration.nanosec	= 0;

		this->default_datawriter_qos->lifespan.duration.sec		=0;
		this->default_datawriter_qos->lifespan.duration.nanosec	=0;

		this->default_datawriter_qos->liveliness.kind					= AUTOMATIC_LIVELINESS_QOS;
		this->default_datawriter_qos->liveliness.lease_duration.sec		= 0;
		this->default_datawriter_qos->liveliness.lease_duration.nanosec	= 0;

		this->default_datawriter_qos->ownership.kind	= SHARED_OWNERSHIP_QOS;

		this->default_datawriter_qos->ownership_strength.value	= 0;

		this->default_datawriter_qos->reliability.kind						= RELIABLE_RELIABILITY_QOS;
		this->default_datawriter_qos->reliability.max_blocking_time.sec		= 0;
		this->default_datawriter_qos->reliability.max_blocking_time.nanosec	= 0;
		
		this->default_datawriter_qos->resource_limits.max_instances				= -1;
		this->default_datawriter_qos->resource_limits.max_samples				= -1;
		this->default_datawriter_qos->resource_limits.max_samples_per_instance	= -1;

		this->default_datawriter_qos->transport_priority.value	= 0;

		memset(this->default_datawriter_qos->user_data.value, 0, MAX_SIZE_OF_USER_DATA_QOS_POLICY);

		this->default_datawriter_qos->writer_data_lifecycle.auto_dispose_unregistered_instances	= true;
	}
}
