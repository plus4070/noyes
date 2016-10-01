#ifndef TOPICDESCRIPTION_H_
#define TOPICDESCRIPTION_H_

#include "../Infrastructure/Type/BasicDefinitions.h"
#include "TypeSupport.h"
#include <iostream>

namespace CNU_DDS
{
	class TopicDescription
	{
	protected:
		std::string		topic_name;
		std::string		type_name;
		TopicSeq		related_topic_list;
		TypeSupport*	type;

	public:
		TopicDescription(std::string a_topic_name, std::string a_type_name);
		~TopicDescription(void);

		pDomainParticipant			get_participant(void);
		std::string					get_type_name(void);
		std::string					get_topic_name(void);
	};
}

#endif
