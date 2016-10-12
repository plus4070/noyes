#ifndef INFOTIMESTAMP_H_
#define INFOTIMESTAMP_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class InfoTimestamp : public Submessage
	{
	public:
		Timestamp	timestamp;

	public:
		InfoTimestamp();
		~InfoTimestamp();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
