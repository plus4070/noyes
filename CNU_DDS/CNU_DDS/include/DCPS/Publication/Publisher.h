#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include "../Infrastructure/DomainEntity.h"
#include "../Infrastructure/Type/Duration_t.h"

#include "../../QoS/EntityQos/PublisherQos.h"
#include "../../QoS/EntityQos/DataWriterQos.h"
#include "../../QoS/EntityQos/TopicQos.h"

namespace CNU_DDS
{
	class Publisher : public DomainEntity
	{
	private:
		DataWriterQos*	default_datawriter_qos;
		char			entity_key;
	public:
		DataWriterSeq*	datawriter_list;

	public:	// inherited from Entity
		Publisher();
		~Publisher();

		ReturnCode_t	get_qos(PublisherQos* qos_list);
		ReturnCode_t	set_qos(PublisherQos* qos_list);

		Listener*		get_listener(void);
		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);

		ReturnCode_t	enable(void);

	public:
		pDataWriter			create_datawriter(pTopic a_topic, DataWriterQos* qos, pDataWriterListener a_listener, StatusMask mask);
		void				add_datawriter(pDataWriter writer);
		ReturnCode_t		delete_datawriter(pDataWriter a_datawriter);
		pDataWriter			lookup_datawriter(std::string topic_name);

		ReturnCode_t		suspend_publications(void);
		ReturnCode_t		resume_publications(void);

		ReturnCode_t		begin_coherent_changes(void);
		ReturnCode_t		end_coherent_changes(void);

		ReturnCode_t		wait_for_acknowledgements(Duration_t max_wait);

		pDomainParticipant	get_participant(void);

		ReturnCode_t		delete_contained_entities(void);

		ReturnCode_t		set_default_datawriter_qos(DataWriterQos* qos_list);
		ReturnCode_t		get_default_datawriter_qos(DataWriterQos* qos_list);
		ReturnCode_t		copy_from_topic_qos(DataWriterQos* a_datawriter_qos, TopicQos* a_topic_qos);

	private:
		void				_set_default_datawriter_qos(void);
	};
}

#endif
