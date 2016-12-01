#ifndef SUBMESSAGE_H_
#define SUBMESSAGE_H_

#include "SubmessageHeader.h"
#include "Type/ByteStream.h"

#include "../../Utility/DoublyLinkedList.h"

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
	class CNU_DDS_DLL_API Submessage
	{
	public:
		SubmessageHeader	submessage_header;

	public:
		virtual	~Submessage(){};
		virtual	ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(unsigned char* data)	= 0;
	};

	typedef DoublyLinkedList<Submessage>	SubmessageSeq;
}

#ifdef __cplusplus
}
#endif
#endif
