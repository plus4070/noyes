#ifndef INCLUDE_RTPS_BEHAVIOR_TYPE_ENTITYEVENT_H_
#define INCLUDE_RTPS_BEHAVIOR_TYPE_ENTITYEVENT_H_

namespace CNU_DDS
{
	class EntityEvent
	{
	public:
		unsigned long	target;
		unsigned long	event;
		unsigned long	data;
	};
}


#endif
