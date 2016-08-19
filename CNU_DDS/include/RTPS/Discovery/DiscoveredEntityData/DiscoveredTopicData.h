#ifndef DISCOVEREDTOPICDATA_H_
#define DISCOVEREDTOPICDATA_H_

#include "../../../BuiltinTopic/TopicBuiltinTopicData.h"

namespace CNU_DDS
{
	class DiscoveredTopicData : public TopicBuiltinTopicData
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
