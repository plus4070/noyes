#ifndef TOPICSTATUS_H_
#define TOPICSTATUS_H_

#include "EntityStatus.h"

namespace CNU_DDS
{
	class TopicStatus : public EntityStatus
	{
	public:
		InconsistentTopicStatus	inconsistent_topic;
	};
}

#endif
