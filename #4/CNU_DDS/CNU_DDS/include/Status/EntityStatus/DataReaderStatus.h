#ifndef DATAREADERSTATUS_H_
#define DATAREADERSTATUS_H_

#include "EntityStatus.h"

namespace CNU_DDS
{
	class DataReaderStatus : public EntityStatus
	{
	public:
		RequestedDeadlineMissedStatus	requested_deadline_missed;
		RequestedIncompatibleQosStatus	requested_incompatile_qos;
		SampleRejectedStatus			sample_rejected;
		LivelinessChangedStatus			liveliness_changed;
		SubscriptionMatchedStatus		subscription_matched;
		SampleLostStatus				sample_lost;
	};
}

#endif
