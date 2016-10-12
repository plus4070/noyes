#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Header.h"
#include "Submessage.h"

namespace CNU_DDS
{
	class Message
	{
	public:
		Header			header;
		SubmessageSeq*	submessages;

	public:
		Message();
		~Message();

		ByteStream*	Serialize();
		bool		Deserialize(ByteStream*	data);
	};
}

#endif
