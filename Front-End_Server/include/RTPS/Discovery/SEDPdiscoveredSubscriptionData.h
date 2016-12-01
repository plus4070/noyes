#ifndef INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_
#define INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredReaderData.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API SEDPdiscoveredSubscriptionData : public DiscoveredReaderData
	{
	public:
		SEDPdiscoveredSubscriptionData();
		~SEDPdiscoveredSubscriptionData();

		ReaderProxy*	getReaderProxyInstance();
		ByteStream*		Serialize();
		void			Deserialize(ByteStream* stream);
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDSUBSCRIPTIONDATA_H_ */
