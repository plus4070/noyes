#ifndef TOPIC_H_
#define TOPIC_H_

#include "TopicDescription.h"
#include "../Infrastructure/DomainEntity.h"
#include "../../QoS/EntityQos/TopicQos.h"
#include "../../Status/Status/InconsistentTopicStatus.h"

namespace CNU_DDS
{
	class Topic : public DomainEntity, public TopicDescription
	{
	public:
		StatusConditionSeq	related_status_condition_list;

	public:	// inherited from Entity
		ReturnCode_t	get_qos(TopicQos* qos_list);
		ReturnCode_t	set_qos(TopicQos* qos_list);

		Listener*		get_listener(void);
		ReturnCode_t	set_listener(Listener* a_listener, StatusMask mask);

		ReturnCode_t	enable(void);

	public:
		Topic(std::string a_topic_name, std::string a_type_name);
		~Topic();

		ReturnCode_t		get_inconsistent_topic_status(InconsistentTopicStatus status);
		TopicDescription*	get_topic_description(void);
	};
}

#endif
