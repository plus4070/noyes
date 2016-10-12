#ifndef INFOREPLY_H_
#define INFOREPLY_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class InfoReply : public Submessage
	{
	public:
		LocatorList	unicast_locator_list;
		LocatorList	multicast_locator_list;

	public:
		InfoReply();
		~InfoReply();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
