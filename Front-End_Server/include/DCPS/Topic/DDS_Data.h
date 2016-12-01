#ifndef DDS_DATA_H_
#define DDS_DATA_H_

#include "../../Utility/DoublyLinkedList.h"
#include "../../RTPS/Messages/Type/ByteStream.h"

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
	class CNU_DDS_DLL_API DDS_Data
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};

	typedef	DoublyLinkedList<DDS_Data>	DataList;
}

#ifdef __cplusplus
}
#endif
#endif
