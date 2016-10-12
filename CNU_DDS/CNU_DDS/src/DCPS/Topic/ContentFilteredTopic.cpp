#include "../../../include/DCPS/Topic/ContentFilteredTopic.h"
#include "../../../include/DCPS/Domain/DomainParticipant.h"

namespace CNU_DDS
{
	ContentFilteredTopic::ContentFilteredTopic(Topic* a_topic, std::string a_filter_expression):TopicDescription(a_topic->get_topic_name(), a_topic->get_type_name())
	{
		related_topic_list.insertInRear(a_topic);
		filter_expression	= a_filter_expression;
	}

	DomainParticipant*	ContentFilteredTopic::get_participant(void)
	{
		DomainParticipant*	result;

		return	result;
	}

	Topic*			ContentFilteredTopic::get_related_topic(void)
	{
		Topic*	result;

		return	result;
	}

	ReturnCode_t	ContentFilteredTopic::get_expression_parameters(StringSeq* expression_parameters)
	{
		ReturnCode_t	result;

		return	result;
	}

	ReturnCode_t	ContentFilteredTopic::set_expression_parameters(StringSeq* expression_parameters)
	{
		ReturnCode_t	result;

		return result;
	}
}
