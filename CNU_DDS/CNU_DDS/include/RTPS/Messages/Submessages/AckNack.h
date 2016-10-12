#ifndef ACKNACK_H_
#define ACKNACK_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class AckNack : public Submessage
	{
	public:
		EntityId			reader_id;
		EntityId			writer_id;
		SequenceNumberSet	reader_sn_state;
		Count				count;

	public:
		AckNack();
		~AckNack();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
