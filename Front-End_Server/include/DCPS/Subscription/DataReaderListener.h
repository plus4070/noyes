#ifndef DATAREADERLISTENER_H_
#define DATAREADERLISTENER_H_

#include "../../Status/Listener.h"
#include "../Infrastructure/Type/BasicDefinitions.h"
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
	class CNU_DDS_DLL_API DataReaderListener : public Listener
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

#ifdef __cplusplus
}
#endif
#endif
