#include "../../../include/Utility/EventProcessor/Event.h"

namespace CNU_DDS
{
	Event::Event()
	{
		arg		= 0;
	}

	Event::Event(unsigned long event_type, unsigned long event_arg)
	{
		type	= event_type;
		arg		= event_arg;
	}

	Event::~Event()
	{

	}
}
