#ifndef INCLUDE_UTILITY_EVENTPROCESSOR_EVENTPROCESSOR_H_
#define INCLUDE_UTILITY_EVENTPROCESSOR_EVENTPROCESSOR_H_

#include "../PlatformOrientedInterface.h"
#include "../DoublyLinkedList.h"
#include "Event.h"

namespace CNU_DDS
{
	class EventProcessor
	{
	private:
		THREAD_TYPE_NATIVE			thread;
		CONDITION_TYPE_NATIVE		condition;
		MUTEX_TYPE_NATIVE			mutex;

	protected:
		bool						is_running;
		bool						is_processing;
		DoublyLinkedList<Event>*	events;

	public:
		EventProcessor();
		virtual			~EventProcessor();

		void			start();
		void			stop();

		void			setEvent(Event* a_event);

		virtual	void	process();

	protected:
		void			waitEvents();

	private:
		static void*	processing(void* arg);
	};
}

#endif /* INCLUDE_UTILITY_EVENTPROCESSOR_EVENTPROCESSOR_H_ */
