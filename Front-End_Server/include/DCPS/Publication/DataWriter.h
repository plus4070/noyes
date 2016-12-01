#ifndef DATAWRITER_H_
#define DATAWRITER_H_

#include "../Infrastructure/DomainEntity.h"
#include "../Infrastructure/Type/Time_t.h"
#include "../Infrastructure/Type/Duration_t.h"

#include "../Topic/Topic.h"
#include "../Topic/DDS_Data.h"

#include "../../Status/Status/InclusionsOfStatuses.h"

#include "../../BuiltinTopic/SubscriptionBuiltinTopicData.h"

#include "../../QoS/EntityQos/DataWriterQos.h"

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
	class CNU_DDS_DLL_API DataWriter : public DomainEntity
	{
	public:
		pPublisher	related_publisher;
		Topic*		related_topic;
		DDS_Data*	related_data;
		pWriter		related_rtps_writer;

	public:	// inherited from Entity
		DataWriter();
		~DataWriter();

		ReturnCode_t	get_qos(DataWriterQos* qos_list);
		ReturnCode_t	set_qos(DataWriterQos* qos_list);

		Listener*		get_listener(void);
		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);

		ReturnCode_t	enable(void);

	public:

		InstanceHandle_t	register_instance(DDS_Data* instance);
		InstanceHandle_t	register_instance_w_timestamp(DDS_Data* instance, Time_t timestamp);

		ReturnCode_t		unregister_instance(DDS_Data* instance, InstanceHandle_t handle);
		ReturnCode_t		unregister_instance_w_timestamp(DDS_Data* instance, InstanceHandle_t handle, Time_t timestamp);

		ReturnCode_t		get_key_value(DDS_Data* key_holder, InstanceHandle_t handle);

		InstanceHandle_t	lookup_instance(DDS_Data* instance);

		ReturnCode_t		write(DDS_Data* data, InstanceHandle_t handle);
		ReturnCode_t		write_w_timestamp(DDS_Data* data, InstanceHandle_t handle, Time_t timestamp);

		ReturnCode_t		dispose(DDS_Data* data, InstanceHandle_t handle);
		ReturnCode_t		disopose_w_timestamp(DDS_Data* data, InstanceHandle_t handle, Time_t timestamp);

		ReturnCode_t		wait_for_acknowledgements(Duration_t max_wait);
		ReturnCode_t		assert_liveliness(void);

		ReturnCode_t		get_liveliness_lost_status(LivelinessLostStatus status);
		ReturnCode_t		get_offered_deadline_missed_status(OfferedDeadlineMissedStatus status);
		ReturnCode_t		get_offered_incompatible_qos_status(OfferedIncompatibleQosStatus status);
		ReturnCode_t		get_publication_matched_stataus(PublicationMatchedStatus status);

		pTopic				get_topic(void);
		pPublisher			get_publisher(void);

		ReturnCode_t		get_matched_subscription_data(SubscriptionBuiltinTopicData* subscription_data, InstanceHandle_t subscription_handle);
		ReturnCode_t		get_matched_subscriptions(InstanceHandleSeq* subscription_handles);
	};
}

#ifdef __cplusplus
}
#endif
#endif
