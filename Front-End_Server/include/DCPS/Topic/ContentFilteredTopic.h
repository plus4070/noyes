#ifndef CONTENTFILTEREDTOPIC_H_
#define CONTENTFILTEREDTOPIC_H_

#include <iostream>

#include "TopicDescription.h"
#include "Topic.h"

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
	class CNU_DDS_DLL_API ContentFilteredTopic : public TopicDescription
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

#ifdef __cplusplus
}
#endif
#endif
