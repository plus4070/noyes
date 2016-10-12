#ifndef DATAREADERLISTENER_H_
#define DATAREADERLISTENER_H_

#include "../../Status/Listener.h"
#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../../Status/Status/InclusionsOfStatuses.h"

namespace CNU_DDS
{
	class DataReaderListener : public Listener
	{
	public:
		//virtual ~DataReaderListener()	= 0;
	public:
		virtual void	on_data_available(pDataReader the_reader)															= 0;
		virtual void	on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)							= 0;
		virtual void	on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)						= 0;
		virtual void	on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)			= 0;
		virtual void	on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status)		= 0;
		virtual void	on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)					= 0;
		virtual void	on_sample_lost(pDataReader the_reader, SampleLostStatus* status)									= 0;
	};
}

#endif
