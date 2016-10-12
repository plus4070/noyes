#ifndef APPLICATION_DCPS_THROUGHPUTLISTENER_H_
#define APPLICATION_DCPS_THROUGHPUTLISTENER_H_

#include "../../include/DCPS/Subscription/DataReaderListener.h"

namespace CNU_DDS
{
	static FILE*			file_pointer;
	static char				chr_file_name[128];

	class ThroughputListener: public DataReaderListener
	{
	public:
		static unsigned long	listener_count;
		static unsigned long	over_count;

		unsigned long	limit;
		unsigned long	count;

		unsigned long	time_start;
		unsigned long	time_end;

		FILE*			fp;

	public:
		ThroughputListener();
		~ThroughputListener();

		void	on_data_available(pDataReader the_reader);
		void	on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status);
		void	on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status);
		void	on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status);
		void	on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status);
		void	on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status);
		void	on_sample_lost(pDataReader the_reader, SampleLostStatus* status);
	};
}

#endif
