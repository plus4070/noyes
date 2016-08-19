#include "../../../include/DCPS/Topic/MultiTopic.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"

namespace CNU_DDS
{
	MultiTopic::MultiTopic(std::string a_topic_name, std::string a_type_name, std::string subscription_expression):TopicDescription(a_topic_name, a_type_name)
	{

	}

	DomainParticipant*	MultiTopic::get_participant(void)
	{
		DomainParticipant*	result;

		return	result;
	}

	ReturnCode_t	MultiTopic::get_expression_parameters(StringSeq* expression_parameters)
	{
		ReturnCode_t	result;

		return	result;
	}

	ReturnCode_t	MultiTopic::set_expression_parameters(StringSeq* expression_parameters)
	{
		ReturnCode_t	result;

		return	result;
	}
}
