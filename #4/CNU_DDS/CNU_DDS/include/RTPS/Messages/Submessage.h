#ifndef SUBMESSAGE_H_
#define SUBMESSAGE_H_

#include "SubmessageHeader.h"
#include "Type/ByteStream.h"

#include "../../Utility/DoublyLinkedList.h"

namespace CNU_DDS
{
	class Submessage
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

#endif
