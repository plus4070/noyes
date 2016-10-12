#ifndef INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDTOPICDATA_H_
#define INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDTOPICDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredTopicData.h"

namespace CNU_DDS
{
	class SEDPdiscoveredTopicData : public DiscoveredTopicData
	{
	public:
		ByteStream*	Serialize();
		void		Deserialize(ByteStream* stream);
	};
}

#endif /* INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDTOPICDATA_H_ */
