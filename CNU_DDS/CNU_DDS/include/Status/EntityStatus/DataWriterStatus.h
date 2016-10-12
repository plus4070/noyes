#ifndef DATAWRITERSTATUS_H_
#define DATAWRITERSTATUS_H_

#include "EntityStatus.h"

namespace CNU_DDS
{
	class DataWriterStatus : public EntityStatus
	{
	public:
		OfferedDeadlineMissedStatus		offered_deadline_missed;
		OfferedIncompatibleQosStatus	offered_incompatible_qos;
		LivelinessLostStatus			liveliness_lost;
		PublicationMatchedStatus		publication_matched;
	};
}

#endif
