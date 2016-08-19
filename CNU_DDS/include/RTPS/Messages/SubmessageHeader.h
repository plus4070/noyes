#ifndef SUBMESSAGEHEADER_H_
#define SUBMESSAGEHEADER_H_

#include "Type/SubmessageKind.h"
#include "Type/SubmessageFlag.h"


namespace CNU_DDS
{
	class SubmessageHeader
	{
	public:
		SubmessageKind	submessage_id;
		SubmessageFlag	flags;
		unsigned short	submessage_length;
	};
}

#endif
