#ifndef CONTENTFILTERPROPERTY_T_H_
#define CONTENTFILTERPROPERTY_T_H_

#include <iostream>

#include "../../../DCPS/Infrastructure/Type/BasicDefinitions.h"

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
	class CNU_DDS_DLL_API ContentFilterProperty_t
	{
		std::string	content_filtered_topic_name;
		std::string	related_topic_name;
		std::string	filter_class_name;
		std::string	filter_expression;
		StringSeq	expression_parameters;
	};
}

#ifdef __cplusplus
}
#endif
#endif
