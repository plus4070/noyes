#ifndef CONTENTFILTEREDTOPIC_H_
#define CONTENTFILTEREDTOPIC_H_

#include <iostream>

#include "TopicDescription.h"
#include "Topic.h"

namespace CNU_DDS
{
	class ContentFilteredTopic : public TopicDescription
	{
	protected:
		std::string	filter_expression;

	public:
		pDomainParticipant	get_participant(void);

	public:
		ContentFilteredTopic(Topic* a_topic, std::string a_filter_expression);

		Topic*			get_related_topic(void);
		ReturnCode_t	get_expression_parameters(StringSeq* expression_parameters);

		ReturnCode_t	set_expression_parameters(StringSeq* expression_parameters);
	};
}

#endif
