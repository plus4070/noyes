#ifndef INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_
#define INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_

#include "../Structure/Type/Duration_t.h"
#include "../Messages/Type/ByteStream.h"

#include "DiscoveredEntityData/DiscoveredWriterData.h"

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
	class CNU_DDS_DLL_API SEDPdiscoveredPublicationData : public DiscoveredWriterData
	{
	public:
		SEDPdiscoveredPublicationData();

		WriterProxy*	getWriterProxyInstance();

		ByteStream*	Serialize();
		void		Deserialize(ByteStream* stream);
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_RTPS_DISCOVERY_SEDPDISCOVEREDPUBLICATIONDATA_H_ */
