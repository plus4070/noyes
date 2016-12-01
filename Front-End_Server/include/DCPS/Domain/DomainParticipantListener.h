#ifndef DOMAINPARTICIPANTLISTENER_H_
#define DOMAINPARTICIPANTLISTENER_H_

#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../../Status/Listener.h"
#include "../../Status/Status/InclusionsOfStatuses.h"

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
	class CNU_DDS_DLL_API DomainParticipantListener : public Listener
	{
	public:
		virtual void	on_inconsistent_topic(pTopic the_topic, InconsistentTopicStatus* status)					= 0;
		virtual void	on_liveliness_lost(pDataWriter the_writer, LivelinessLostStatus* status)					= 0;
		virtual void	on_offered_deadline_missed(pDataWriter	the_writer, OfferedDeadlineMissedStatus* status)	= 0;
		virtual void	on_offered_incompatible_qos(pDataWriter	the_writer, OfferedIncompatibleQosStatus* status)	= 0;
		virtual void	on_data_on_readers(pSubscriber the_subscriber)												= 0;
		virtual void	on_sample_lost(pDataReader the_reader, SampleLostStatus* status)							= 0;
		virtual void	on_data_available(pDataReader the_reader)													= 0;
		virtual void	on_sample_rejected(pDataReader the_reader, SampleRejectedStatus* status)					= 0;
		virtual void	on_liveliness_changed(pDataReader the_reader, LivelinessChangedStatus* status)				= 0;
		virtual void	on_requested_deadline_missed(pDataReader the_reader, RequestedDeadlineMissedStatus* status)	= 0;
		virtual void	on_requested_incompatible_qos(pDataReader, RequestedIncompatibleQosStatus* status)			= 0;
		virtual void	on_publication_matched(pDataWriter the_writer, PublicationMatchedStatus* status)			= 0;
		virtual void	on_subscription_matched(pDataReader the_reader, SubscriptionMatchedStatus* status)			= 0;
	};
}

#ifdef __cplusplus
}
#endif
#endif
