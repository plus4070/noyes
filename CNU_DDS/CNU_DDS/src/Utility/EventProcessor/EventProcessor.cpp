#include "../../../include/Utility/EventProcessor/EventProcessor.h"
#include <stdio.h>

namespace CNU_DDS
{
	EventProcessor::EventProcessor()
	{
		is_running		= false;
		is_processing	= false;

		create_condition(&condition);
		create_mutex(&mutex);

		events	= new DoublyLinkedList<Event>();
	}

	EventProcessor::~EventProcessor()
	{
		destroy_condition(&condition);
		destroy_mutex(&mutex);
		destroy_thread(&thread);

		events->deleteAll();
		delete(events);
	}

	void			EventProcessor::start()
	{
		if(is_running == false)
		{
			is_running	= true;
			create_thread(&thread, processing, this);
		}
	}

	void			EventProcessor::stop()
	{
		is_running	= false;
	}

	void			EventProcessor::setEvent(Event* a_event)
	{
		events->insertInRear(a_event);

		if(!is_processing)
		{
			condition_signal(&condition);
		}
	}

	void			EventProcessor::waitEvents()
	{
		is_processing	= false;
		condition_wait(&condition, &mutex);
		is_processing	= true;
	}

	void			EventProcessor::process()
	{

	}

	void*			EventProcessor::processing(void* arg)
	{
		EventProcessor*	processor	= (EventProcessor*)arg;

		while(processor->is_running)
		{
			processor->process();
		}

		return NULL;
	}
}
