#include "../../../include/DCPS/Domain/DomainParticipantFactory.h"

namespace CNU_DDS
{
	DomainParticipantFactory::DomainParticipantFactory()
	{

	}

	DomainParticipantFactory*	DomainParticipantFactory::get_instance()
	{
		extern DomainParticipantFactory*	DOMAIN_PARTICIPANT_FACTORY;

		DomainParticipantFactory*	result	= NULL;

		if(DOMAIN_PARTICIPANT_FACTORY == NULL)
		{
			DOMAIN_PARTICIPANT_FACTORY	= new DomainParticipantFactory();
		}

		result	= DOMAIN_PARTICIPANT_FACTORY;

		return result;
	}

	DomainParticipant*			DomainParticipantFactory::create_participant(DomainId_t domain_id, DomainParticipantQos* qos_list, DomainParticipantListener* a_listener, StatusMask mask)
	{
		DomainParticipant*	result;

		if(qos_list == NULL)
		{
			if(get_default_participant_qos(qos_list) != RETCODE_OK)
			{
				return NULL;
			}
		}

		result	= new DomainParticipant(domain_id, 0);

		result->set_qos(qos_list);

		if( (qos != NULL) && ((DomainParticipantFactoryQos*)qos)->entity_factory.auto_enable_created_entities )
		{
			result->enable();
		}

		participants.insertInRear(result);

		return result;
	}

	ReturnCode_t				DomainParticipantFactory::delete_participant(DomainParticipant* a_participant)
	{
		ReturnCode_t	result	= RETCODE_PRECONDITION_NOT_MET;
		int				i	= 0;

		Node<DomainParticipant>*	node	= participants.getNodeByIndex(0);	/* getting the head-node of list */

		while(participants.getSize() > i++)
		{
			node	= node->rear;

			if(node->value == a_participant)
			{
				if(false)	/* The participant includes domain entities */
				{
					result	= RETCODE_PRECONDITION_NOT_MET;
					break;
				}
				else
				{
					participants.cutNode(node);
					delete(node->value);
					delete(node);
					result	= RETCODE_OK;
					break;
				}
			}
		}

		return result;
	}

	DomainParticipant*			DomainParticipantFactory::lookup_participant(DomainId_t domain_id)
	{
		DomainParticipant*	result	= NULL;
		int					i		= 0;

		Node<DomainParticipant>*	node	= participants.getNodeByIndex(0);	/* getting the head-node of list */

		while(participants.getSize() > i++)
		{
			node	= node->rear;

			if(node->value->get_domain_id() == domain_id)
			{
				result	= node->value;
			}
		}

		return result;
	}

	ReturnCode_t				DomainParticipantFactory::set_default_participant_qos(DomainParticipantQos* qos_list)
	{
		ReturnCode_t	result;

		/* Checks that resulting policies are self consistent */

		if(false)	/* if there are inconsistent policies */
		{
			result	= RETCODE_INCONSISTENT_POLICY;
		}
		else
		{
			default_participant_qos	= qos_list;

			result	= RETCODE_OK;
		}

		return result;
	}

	ReturnCode_t				DomainParticipantFactory::get_default_participant_qos(DomainParticipantQos* qos_list)
	{
		ReturnCode_t	result;

		if(default_participant_qos == NULL)	/* if the DomainParticipantFactory::set_default_participant_qos method had never been called */
		{
			/* set the default values listed in the QoS table in Section 7.1.3 DCPS Specification */

			result		= RETCODE_OK;
		}
		else
		{
			qos_list	= default_participant_qos;

			result		= RETCODE_OK;
		}

		return result;
	}

	ReturnCode_t				DomainParticipantFactory::set_qos(DomainParticipantFactoryQos* qos_list)
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

	ReturnCode_t				DomainParticipantFactory::get_qos(DomainParticipantFactoryQos* qos_list)
	{
		ReturnCode_t	result;

		if(qos)
		{
			qos_list	= qos;
			result		= RETCODE_OK;
		}
		else
		{
			qos_list	= NULL;
			result		= RETCODE_PRECONDITION_NOT_MET;
		}

		return result;
	}
}
