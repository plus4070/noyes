#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "Header.h"
#include "Submessage.h"
#include "Message.h"

#include "../Structure/Type/Locator_t.h"

#include "Type/Time_t.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API Receiver
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

#ifdef __cplusplus
}
#endif
#endif
