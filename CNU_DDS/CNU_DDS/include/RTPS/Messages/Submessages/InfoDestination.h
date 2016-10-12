#ifndef INFODESTINATION_H_
#define INFODESTINATION_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class InfoDestination : public Submessage
	{
	public:
		GuidPrefix	guid_prefix;

	public:
		InfoDestination();
		~InfoDestination();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
