#ifndef HEARTBEATFRAG_H_
#define HEARTBEATFRAG_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class HeartbeatFrag : public Submessage
	{
	public:
		EntityId		reader_id;
		EntityId		writer_id;
		SequenceNumber	writer_sn;
		FragmentNumber	last_fragment_num;
		Count			count;

	public:
		HeartbeatFrag();
		~HeartbeatFrag();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
