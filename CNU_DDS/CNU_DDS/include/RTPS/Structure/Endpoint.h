#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include "RTPSEntity.h"

#include "Type/Locator_t.h"
#include "Type/ReliabilityKind_t.h"
#include "Type/TopicKind_t.h"
#include "Type/EntityEvent.h"


namespace CNU_DDS
{
	class Endpoint : public RTPSEntity
	{
	public:
		LocatorSeq			unicast_locator_list;
		LocatorSeq			multicast_locator_list;
		ReliabilityKind_t	reliability_level;
		TopicKind_t			topic_kind;

	protected:
		MUTEX_TYPE_NATIVE				event_mutex;
		CONDITION_TYPE_NATIVE			event_condition;
		DoublyLinkedList<EntityEvent>*	events;
		bool							is_waiting;

	public:
		Endpoint();
		~Endpoint();

		void		Wait();
		void		SetEvent(EntityEvent* event);
	};

	typedef DoublyLinkedList<Endpoint>	EndpointSeq;
}

#endif
