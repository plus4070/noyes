#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class Heartbeat : public Submessage
	{
	public:
		EntityId		reader_id;
		EntityId		writer_id;
		SequenceNumber	first_sn;
		SequenceNumber	last_sn;
		Count			count;

	public:
		Heartbeat();
		~Heartbeat();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
