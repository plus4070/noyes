#ifndef NACKFRAG_H_
#define NACKFRAG_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class NackFrag : public Submessage
	{
	public:
		EntityId			reader_id;
		EntityId			writer_id;
		SequenceNumber		writer_sn;
		FragmentNumberSet	fragment_number_state;
		Count				count;

	public:
		NackFrag();
		~NackFrag();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
