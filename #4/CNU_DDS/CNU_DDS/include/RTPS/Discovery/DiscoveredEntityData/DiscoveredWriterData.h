#ifndef DISCOVEREDWRITERDATA_H_
#define DISCOVEREDWRITERDATA_H_

#include "../../../BuiltinTopic/PublicationBuiltinTopicData.h"

#include "../../Behavior/WriterProxy.h"

namespace CNU_DDS
{
	class DiscoveredWriterData : public PublicationBuiltinToipcData, public WriterProxy
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
