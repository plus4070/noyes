#ifndef DISCOVEREDWRITERDATA_H_
#define DISCOVEREDWRITERDATA_H_

#include "../../../BuiltinTopic/PublicationBuiltinTopicData.h"

#include "../../Behavior/WriterProxy.h"

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
	class CNU_DDS_DLL_API DiscoveredWriterData : public PublicationBuiltinToipcData, public WriterProxy
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};
}

#ifdef __cplusplus
}
#endif
#endif
