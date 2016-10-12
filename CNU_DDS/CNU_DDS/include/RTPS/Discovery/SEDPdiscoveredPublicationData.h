#ifndef INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_
#define INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredWriterData.h"

namespace CNU_DDS
{
	class SEDPdiscoveredPublicationData : public DiscoveredWriterData
	{
	public:
		SEDPdiscoveredPublicationData();

		WriterProxy*	getWriterProxyInstance();

		ByteStream*	Serialize();
		void		Deserialize(ByteStream* stream);
	};
}

#endif /* INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_ */
