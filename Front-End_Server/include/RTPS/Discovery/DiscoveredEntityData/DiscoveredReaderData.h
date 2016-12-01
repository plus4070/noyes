#ifndef DISCOVEREDREADERDATA_H_
#define DISCOVEREDREADERDATA_H_

#include "../../../BuiltinTopic/SubscriptionBuiltinTopicData.h"

#include "../../Behavior/ReaderProxy.h"

#include "../Type/ContentFilterProperty_t.h"

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
	class CNU_DDS_DLL_API DiscoveredReaderData : public SubscriptionBuiltinTopicData, public ReaderProxy
	{
	public:
		ContentFilterProperty_t	content_filter;

	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#ifdef __cplusplus
}
#endif
#endif
