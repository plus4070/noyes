#ifndef DATA_H_
#define DATA_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class Data : public Submessage
	{
	public:
		unsigned char	extraFlags[2];
		EntityId		reader_id;
		EntityId		writer_id;
		SequenceNumber	writer_sn;
		ParameterList	inline_qos;
		SerializedData	serialized_payload;

	public:
		Data();
		~Data();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
