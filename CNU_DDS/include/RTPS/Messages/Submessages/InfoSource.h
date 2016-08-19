#ifndef INFOSOURCE_H_
#define INFOSOURCE_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

namespace CNU_DDS
{
	class InfoSource : public Submessage
	{
	public:
		ProtocolVersion	protocol_version;
		VendorId		vendor_id;
		GuidPrefix		guid_prefix;

	public:
		InfoSource();
		~InfoSource();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#endif
