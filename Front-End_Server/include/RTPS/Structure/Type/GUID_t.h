#ifndef GUID_T_H_
#define GUID_T_H_

#include "GuidPrefix_t.h"
#include "EntityId_t.h"

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
	class CNU_DDS_DLL_API GUID_t
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

#ifdef __cplusplus
}
#endif
#endif
