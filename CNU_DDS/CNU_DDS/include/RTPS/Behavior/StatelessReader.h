#ifndef STATELESSREADER_H_
#define STATELESSREADER_H_

#include "../Structure/Reader.h"
#include "../../Utility/PlatformOrientedInterface.h"

namespace CNU_DDS
{
	class StatelessReader : public Reader
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

#endif
