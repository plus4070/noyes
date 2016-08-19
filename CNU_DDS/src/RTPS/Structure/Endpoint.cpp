#include "../../../include/RTPS/Structure/Endpoint.h"

namespace CNU_DDS
{
	Endpoint::Endpoint()
	{
		is_waiting	= false;

		create_mutex(&event_mutex);
		create_condition(&event_condition);

		events	= new DoublyLinkedList<EntityEvent>();
	}

	Endpoint::~Endpoint()
	{
		destroy_mutex(&event_mutex);
		destroy_condition(&event_condition);

		delete(events);
	}

	void		Endpoint::Wait()
	{
		is_waiting	= true;
		condition_wait(&event_condition, &event_mutex);
		is_waiting	= false;
	}

	void		Endpoint::SetEvent(EntityEvent* event)
	{
		events->insertInRear(event);

		if(is_waiting)
		{
			condition_signal(&event_condition);
		}
	}
}
