#ifndef INCLUDE_DCPS_BUILTINENTITY_BUILTINPARTICIPANTSREADERLISTENER_H_
#define INCLUDE_DCPS_BUILTINENTITY_BUILTINPARTICIPANTSREADERLISTENER_H_

#include "../Subscription/DataReaderListener.h"

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
	class CNU_DDS_DLL_API BuiltinParticipantsReaderListener: public DataReaderListener
	{
	public:
		BuiltinParticipantsReaderListener();
		~BuiltinParticipantsReaderListener();

		void	on_data_available(pDataReader the_reader);
		void	on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status);
		void	on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status);
		void	on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status);
		void	on_requested_incompatible_qos(pDataReader the_reader, RequestedIncompatibleQosStatus* status);
		void	on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status);
		void	on_sample_lost(pDataReader the_reader, SampleLostStatus* status);
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_DCPS_BUILTINENTITY_BUILTINPARTICIPANTSREADERLISTENER_H_ */
