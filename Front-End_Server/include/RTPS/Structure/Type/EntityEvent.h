#ifndef INCLUDE_RTPS_BEHAVIOR_TYPE_ENTITYEVENT_H_
#define INCLUDE_RTPS_BEHAVIOR_TYPE_ENTITYEVENT_H_

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
	class CNU_DDS_DLL_API EntityEvent
	{
	public:
		unsigned long	target;
		unsigned long	event;
		unsigned long	data;
	};
}

#ifdef __cplusplus
}
#endif
#endif
