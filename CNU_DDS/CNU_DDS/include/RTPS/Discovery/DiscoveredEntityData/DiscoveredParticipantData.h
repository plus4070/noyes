#ifndef DISCOVEREDPARTICIPANTDATA_H_
#define DISCOVEREDPARTICIPANTDATA_H_

#include "../../../BuiltinTopic/ParticipantBuiltinTopicData.h"
#include "../ParticipantProxy.h"

namespace CNU_DDS
{
	class DiscoveredParticipantData : public ParticipantBuiltinTopicData, public ParticipantProxy
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
