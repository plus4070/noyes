#ifndef STATELESSREADER_H_
#define STATELESSREADER_H_

#include "../Structure/Reader.h"
#include "../../Utility/PlatformOrientedInterface.h"

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
	class CNU_DDS_DLL_API StatelessReader : public Reader
	{
	private:
		THREAD_TYPE_NATIVE				processing_thread;
		bool							is_running;

	public:
		StatelessReader();
		StatelessReader(GUID_t a_guid, ReliabilityKind_t reliability);
		~StatelessReader();

	public:
		static void*	processing(void* arg);
		void			start();
	};
}

#ifdef __cplusplus
}
#endif
#endif
