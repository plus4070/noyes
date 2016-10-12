#ifndef TYPESUPPORT_H_
#define TYPESUPPORT_H_

#include "../Infrastructure/Type/BasicDefinitions.h"
#include "../Infrastructure/Type/ReturnCode_t.h"

namespace CNU_DDS
{
	class TypeSupport
	{
	public:
		virtual ~TypeSupport(void);

		virtual ReturnCode_t	register_type(pDomainParticipant a_participant, std::string type_name);
		virtual std::string		get_type_name(void);
	};
}

#endif
