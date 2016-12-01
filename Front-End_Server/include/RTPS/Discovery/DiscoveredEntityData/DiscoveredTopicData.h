#ifndef DISCOVEREDTOPICDATA_H_
#define DISCOVEREDTOPICDATA_H_

#include "../../../BuiltinTopic/TopicBuiltinTopicData.h"

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
	class CNU_DDS_DLL_API DiscoveredTopicData : public TopicBuiltinTopicData
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
