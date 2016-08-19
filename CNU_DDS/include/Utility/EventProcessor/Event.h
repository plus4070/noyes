
#ifndef INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_
#define INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_

namespace CNU_DDS
{
	class Event
	{
	public:
		unsigned long	type;
		unsigned long	arg;

	public:
		Event();
		Event(unsigned long event_type, unsigned long event_arg);
		~Event();
	};
}

#endif /* INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_ */
