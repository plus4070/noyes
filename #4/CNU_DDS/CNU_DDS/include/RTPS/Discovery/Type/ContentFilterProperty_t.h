#ifndef CONTENTFILTERPROPERTY_T_H_
#define CONTENTFILTERPROPERTY_T_H_

#include <iostream>

#include "../../../DCPS/Infrastructure/Type/BasicDefinitions.h"

namespace CNU_DDS
{
	class ContentFilterProperty_t
	{
		std::string	content_filtered_topic_name;
		std::string	related_topic_name;
		std::string	filter_class_name;
		std::string	filter_expression;
		StringSeq	expression_parameters;
	};
}

#endif
