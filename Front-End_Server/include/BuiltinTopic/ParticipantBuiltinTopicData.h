#ifndef PARTICIPANTBUILTINTOPICDATA_H_
#define PARTICIPANTBUILTINTOPICDATA_H_

#include "../QoS/QosPolicies.h"
#include "Type/BuiltinTopicKey_t.h"
#include "../DCPS/Topic/DDS_Data.h"

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
	class CNU_DDS_DLL_API ParticipantBuiltinTopicData: public DDS_Data
	{
	private:	// It has been defined as private in a Discovery Module in RTPS Specification.
		BuiltinTopicKey_t	key;
		UserDataQosPolicy	user_data;

	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};

	typedef DoublyLinkedList<ParticipantBuiltinTopicData>	ParticipantBuiltinTopicDataSeq;
}

#ifdef __cplusplus
}
#endif
#endif
