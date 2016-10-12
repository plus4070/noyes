#include "../../../include/DCPS/Topic/TopicDescription.h"

namespace CNU_DDS
{
		TopicDescription::TopicDescription(std::string a_topic_name, std::string a_type_name)
		{
			topic_name	= a_topic_name;
			type_name	= a_type_name;
		}

		TopicDescription::~TopicDescription(void)
		{

		}

		pDomainParticipant	TopicDescription::get_participant(void)
		{
			pDomainParticipant result;

			return result;
		}

		std::string			TopicDescription::get_type_name(void)
		{
			return type_name;
		}

		std::string			TopicDescription::get_topic_name(void)
		{
			return topic_name;
		}
}
