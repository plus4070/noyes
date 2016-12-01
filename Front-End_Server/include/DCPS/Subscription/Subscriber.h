#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include "../Infrastructure/DomainEntity.h"

#include "../../QoS/EntityQos/SubscriberQos.h"
#include "../../QoS/EntityQos/DataReaderQos.h"
#include "../../QoS/EntityQos/TopicQos.h"

#include "../../Status/Type/SampleStateKind.h"
#include "../../Status/Type/ViewStateKind.h"
#include "../../Status/Type/InstanceStateKind.h"

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
	class CNU_DDS_DLL_API Subscriber : public DomainEntity
	{
	private:
		DataReaderQos*	default_datareader_qos;
		char			entity_key;

	public:
		DataReaderSeq*	datareader_list;

	public:	// inherited from Entity
		Subscriber();
		~Subscriber();

		ReturnCode_t	get_qos(SubscriberQos* qos_list);
		ReturnCode_t	set_qos(SubscriberQos* qos_list);

		Listener*		get_listener(void);
		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);

		ReturnCode_t	enable(void);

	public:
		pDataReader			create_datareader(pTopicDescription a_topic, DataReaderQos* qos, pDataReaderListener a_listener, StatusMask mask);
		void				add_datareader(pDataReader reader);
		ReturnCode_t		delete_datareader(pDataReader a_datareader);
		pDataReader			lookup_datareader(std::string topic_name);

		ReturnCode_t		begin_access(void);
		ReturnCode_t		end_access(void);

		pDomainParticipant	get_participant(void);
		ReturnCode_t		get_datareaders(DomainEntityList* readers, SampleStateMask sample_states, ViewStateMask view_states, InstanceStateMask instance_states);

		ReturnCode_t		notify_datareaders(void);
		ReturnCode_t		delete_contained_entities(void);

		ReturnCode_t		set_default_datareader_qos(DataReaderQos* qos_list);
		ReturnCode_t		get_default_datareader_qos(DataReaderQos* qos_list);
		ReturnCode_t		copy_from_topic_qos(DataReaderQos* a_datareader_qos, TopicQos* a_topic_qos);

	private:
		void				_set_default_datareader_qos(void);
	};
}

#ifdef __cplusplus
}
#endif
#endif
