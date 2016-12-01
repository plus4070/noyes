
#ifndef INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_
#define INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_

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
	class CNU_DDS_DLL_API Event
	{
	public:
		unsigned long	type;
		unsigned long	arg;

	public:
		Event();
		Event(unsigned long event_type, unsigned long event_arg);
		~Event();
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_EVENTPROCESSOR_EVENT_H_ */
