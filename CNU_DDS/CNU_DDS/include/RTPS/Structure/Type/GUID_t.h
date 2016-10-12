#ifndef GUID_T_H_
#define GUID_T_H_

#include "GuidPrefix_t.h"
#include "EntityId_t.h"

namespace CNU_DDS
{
	class GUID_t
	{
	public:
		GuidPrefix_t	guid_prefix;
		EntityId_t		entity_id;

	public:
		void	operator	= (const GUID_t& arg);
		bool	operator	== (const GUID_t& arg);
	};

	const GUID_t	GUID_UNKNOWN	= {GUIDPREFIX_UNKNOWN, ENTITYID_UNKNOWN};
}


#endif
