#ifndef DISCOVEREDREADERDATA_H_
#define DISCOVEREDREADERDATA_H_

#include "../../../BuiltinTopic/SubscriptionBuiltinTopicData.h"

#include "../../Behavior/ReaderProxy.h"

#include "../Type/ContentFilterProperty_t.h"

namespace CNU_DDS
{
	class DiscoveredReaderData : public SubscriptionBuiltinTopicData, public ReaderProxy
	{
	public:
		ContentFilterProperty_t	content_filter;

	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#endif
