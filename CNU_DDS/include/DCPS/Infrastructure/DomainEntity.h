#ifndef DOMAINENTITY_H_
#define DOMAINENTITY_H_

#include "Entity.h"

namespace CNU_DDS
{
	class DomainEntity : public Entity
	{
	public:
		pDomainParticipant	related_participant;

	public:
		virtual ~DomainEntity();

		virtual ReturnCode_t	set_qos(EntityQos* qos_list);
		virtual	ReturnCode_t	get_qos(EntityQos* qos_list);

		virtual	ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);
		virtual	Listener*		get_listener(void);

		virtual	ReturnCode_t	enable(void);
	};

	typedef	DoublyLinkedList<class DomainEntity>	DomainEntityList;
}

#endif
