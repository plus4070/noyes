#ifndef INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_
#define INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredReaderData.h"

namespace CNU_DDS
{
	class SEDPdiscoveredSubscriptionData : public DiscoveredReaderData
	{
	public:
		SEDPdiscoveredSubscriptionData();
		~SEDPdiscoveredSubscriptionData();

		ReaderProxy*	getReaderProxyInstance();
		ByteStream*		Serialize();
		void			Deserialize(ByteStream* stream);
	};
}

#endif /* INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_ */
