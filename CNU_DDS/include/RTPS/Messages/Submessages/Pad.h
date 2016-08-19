#ifndef PAD_H_
#define PAD_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class Pad : public Submessage
	{
	public:
		Pad();
		~Pad();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
