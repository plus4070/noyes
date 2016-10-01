#ifndef DATAREADER_H_
#define DATAREADER_H_

#include "../Infrastructure/DomainEntity.h"
#include "../Infrastructure/Type/Duration_t.h"

#include "../Topic/DDS_Data.h"

#include "SampleInfo.h"
#include "DataSample.h"

#include "../../Status/Type/SampleStateKind.h"
#include "../../Status/Type/ViewStateKind.h"
#include "../../Status/Type/InstanceStateKind.h"
#include "../../Status/Condition/ReadCondition.h"
#include "../../Status/Condition/QueryCondition.h"

#include "../../BuiltinTopic/PublicationBuiltinTopicData.h"

#include "../../QoS/EntityQos/DataReaderQos.h"

namespace CNU_DDS
{
	class DataReader : public DomainEntity
	{
	public:
		pSubscriber			related_subscriber;
		TopicDescription*	related_topic_description;
		DDS_Data*			related_data;
		pReader				related_rtps_reader;
		DataSampleSeq		data_sample_list;
		ReadConditionSeq	read_condition_list;

		// Supplement
		bool					read_flag; // 데이터를 읽고 있는지 아닌지를 확인.. -> Subscriber에서 필요하므로 임시로 추가했음

	public:	// inherited from Entity
		DataReader();
		~DataReader();

		ReturnCode_t	get_qos(DataReaderQos* qos_list);
		ReturnCode_t	set_qos(DataReaderQos* qos_list);

		Listener*		get_listener(void);
		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);

		ReturnCode_t	enable(void);

	public:
		ReturnCode_t		read(DataList* data_values, SampleInfoList* sample_infos, long max_sample, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		read_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, ReadCondition* a_condition);
		ReturnCode_t		read_next_sample(DDS_Data* data_value, SampleInfo* sample_info);

		ReturnCode_t		take(DataList* data_values, SampleInfoList* sample_infos, long max_sample, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		take_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, ReadCondition* a_condition);
		ReturnCode_t		take_next_sample(DDS_Data* data_value, SampleInfo* sample_info);

		ReturnCode_t		read_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t a_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		read_next_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		read_next_instance_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, ReadCondition* a_condition);

		ReturnCode_t		take_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t a_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		take_next_instance(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		ReturnCode_t		take_next_instance_w_condition(DataList* data_values, SampleInfoList* sample_infos, long max_sample, InstanceHandle_t previous_handle, ReadCondition* a_condition);

		ReturnCode_t		return_loan(DataList* data_values, SampleInfoList* sample_infos);
		ReturnCode_t		get_key_value(DDS_Data* key_holder, InstanceHandle_t handle);

		InstanceHandle_t	lookup_instance(DDS_Data* instance);

		ReadCondition*		create_readcondition(SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);
		QueryCondition*		create_querycondition(SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states, std::string query_expression, StringSeq* query_parameters);

		ReturnCode_t		delete_readcondition(ReadCondition* a_condition);

		ReturnCode_t		get_liveliness_changed_status(LivelinessChangedStatus status);
		ReturnCode_t		get_requested_deadline_missed_status(RequestedDeadlineMissedStatus status);
		ReturnCode_t		get_requested_incompatible_qos_status(RequestedIncompatibleQosStatus status);
		ReturnCode_t		get_sample_lost_status(SampleLostStatus status);
		ReturnCode_t		get_sample_rejected_status(SampleRejectedStatus status);
		ReturnCode_t		get_subscription_matched_status(SubscriptionMatchedStatus status);

		TopicDescription*	get_topic_description(void);
		pSubscriber			get_subscriber(void);

		ReturnCode_t		delete_contained_entities(void);
		ReturnCode_t		wait_for_historical_data(Duration_t max_wait);

		ReturnCode_t		get_matched_publication_data(PublicationBuiltinToipcData* publication_data, InstanceHandle_t publication_handle);
		ReturnCode_t		get_matched_publications(InstanceHandleSeq* publication_handles);
	};
}

#endif
