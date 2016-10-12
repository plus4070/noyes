#ifndef MULTITOPIC_H_
#define MULTITOPIC_H_

#include "TopicDescription.h"
#include "Topic.h"
#include "../Infrastructure/Type/ReturnCode_t.h"

namespace CNU_DDS
{
	class MultiTopic : public TopicDescription
	{
	protected:
		std::string	subscription_expression;

	public:
		pDomainParticipant	get_participant(void);

	public:
		MultiTopic(std::string a_topic_name, std::string a_type_name, std::string subscription_expression);

		ReturnCode_t		get_expression_parameters(StringSeq* expression_parameters);
		ReturnCode_t		set_expression_parameters(StringSeq* expression_parameters);
	};
}

#endif
