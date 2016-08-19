#ifndef SPDPDISCOVEREDPARTICIPANTDATA_H_
#define SPDPDISCOVEREDPARTICIPANTDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredParticipantData.h"

namespace CNU_DDS
{
	class SPDPdiscoveredParticipantData : public DiscoveredParticipantData
	{
	public:
		Duration_t	lease_duration;

	public:
		ByteStream*	Serialize();
		void		Deserialize(ByteStream* stream);

		ParticipantProxy*	getParticipantProxy();
	};
}

#endif
