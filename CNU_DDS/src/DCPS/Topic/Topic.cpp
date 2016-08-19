#include "../../../include/DCPS/Topic/Topic.h"

namespace CNU_DDS
{
	Topic::Topic(std::string a_topic_name, std::string a_type_name):TopicDescription(a_topic_name, a_type_name)
	{

	}

	Topic::~Topic(void)
	{

	}

	ReturnCode_t	Topic::get_qos(TopicQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	Topic::set_qos(TopicQos* qos_list)
	{
		ReturnCode_t	result;

		return result;
	}

	Listener*		Topic::get_listener(void)
	{
		Listener*	result;

		return result;
	}

	ReturnCode_t	Topic::set_listener(Listener* a_listener, StatusMask mask)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	Topic::enable(void)
	{
		ReturnCode_t	result;

		return result;
	}

	ReturnCode_t	Topic::get_inconsistent_topic_status(InconsistentTopicStatus status)
	{
		ReturnCode_t	result;

		return result;
	}

	TopicDescription*	Topic::get_topic_description(void)
	{
		unsigned long		result;

		result	= (unsigned long)this;
		result	+= sizeof(DomainEntity);

		return (TopicDescription*)result;
	}
}
