#ifndef GAP_H_
#define GAP_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class Gap : public Submessage
	{
	public:
		EntityId			reader_id;
		EntityId			writer_id;
		SequenceNumber		gap_start;
		SequenceNumberSet	gap_list;

	public:
		Gap();
		~Gap();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
