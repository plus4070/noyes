#ifndef TOPICLISTENER_H_
#define TOPICLISTENER_H_

#include "../../Status/Listener.h"

namespace CNU_DDS
{
	class TopicListener : public Listener
	{
	public:
		virtual void	on_inconsistent_topic(pTopic theTopic, InconsistentTopicStatus* status)	= 0;
	};
}

#endif
