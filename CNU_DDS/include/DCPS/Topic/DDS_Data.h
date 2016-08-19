#ifndef DDS_DATA_H_
#define DDS_DATA_H_

#include "../../Utility/DoublyLinkedList.h"
#include "../../RTPS/Messages/Type/ByteStream.h"

namespace CNU_DDS
{
	class DDS_Data
	{
	public:
		virtual ByteStream*	Serialize()							= 0;
		virtual void		Deserialize(ByteStream* stream)		= 0;
	};

	typedef	DoublyLinkedList<DDS_Data>	DataList;
}

#endif
