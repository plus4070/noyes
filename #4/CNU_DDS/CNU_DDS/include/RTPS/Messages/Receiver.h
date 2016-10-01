#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Header.h"
#include "Submessage.h"
#include "Message.h"

#include "../Structure/Type/Locator_t.h"

#include "Type/Time_t.h"

namespace CNU_DDS
{
	class Receiver
	{
	private:
		ProtocolVersion_t	source_version;
		VendorId_t			source_vendor_id;
		GuidPrefix_t		source_guid_prefix;
		GuidPrefix_t		destination_guid_prefix;
		LocatorSeq			unicast_reply_locator_list;
		LocatorSeq			multicast_reply_locator_list;
		bool				have_timestamp;
		Time_t				timestamp;
		Message*			received_message;
	};
}

#endif
