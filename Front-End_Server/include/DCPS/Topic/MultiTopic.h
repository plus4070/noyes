#ifndef MULTITOPIC_H_
#define MULTITOPIC_H_

#include "TopicDescription.h"
#include "Topic.h"
#include "../Infrastructure/Type/ReturnCode_t.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace CNU_DDS
{
	class CNU_DDS_DLL_API MultiTopic : public TopicDescription
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

#ifdef __cplusplus
}
#endif
#endif
