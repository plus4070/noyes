#ifndef DOMAINPARTICIPANTFACTORY_H_
#define DOMAINPARTICIPANTFACTORY_H_

#include "DomainParticipant.h"
#include "DomainParticipantListener.h"
#include "../../QoS/EntityQos/DomainParticipantFactoryQos.h"
#include "../../QoS/EntityQos/DomainParticipantQos.h"

namespace CNU_DDS
{
	class DomainParticipantFactory
	{
	private:
		DomainParticipantFactoryQos*	qos;
		DomainParticipantQos*			default_participant_qos;
		DomainParticipantSeq			participants;

	private:
		DomainParticipantFactory();

	public:
		static DomainParticipantFactory*	get_instance();

		DomainParticipant*					create_participant(DomainId_t domain_id, DomainParticipantQos* qos_list, DomainParticipantListener* a_listener, StatusMask mask);
		ReturnCode_t						delete_participant(DomainParticipant* a_participant);

		DomainParticipant*					lookup_participant(DomainId_t domain_id);

		ReturnCode_t						set_default_participant_qos(DomainParticipantQos* qos_list);
		ReturnCode_t						get_default_participant_qos(DomainParticipantQos* qos_list);

		ReturnCode_t						set_qos(DomainParticipantFactoryQos* qos_list);
		ReturnCode_t						get_qos(DomainParticipantFactoryQos* qos_list);
	};

	static	DomainParticipantFactory*	DOMAIN_PARTICIPANT_FACTORY;
}

#endif
