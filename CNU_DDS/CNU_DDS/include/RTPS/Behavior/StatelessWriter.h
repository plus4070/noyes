#ifndef STATELESSWRITER_H_
#define STATELESSWRITER_H_

#include "../Structure/Writer.h"
#include "../../Utility/PlatformOrientedInterface.h"

#include "ReaderLocator.h"

namespace CNU_DDS
{
	class StatelessWriter : public Writer
	{
	private:
		THREAD_TYPE_NATIVE				processing_thread;
		bool							is_running;

	public:
		Duration_t			resend_data_period;
		ReaderLocatorSeq*	reader_locators;

	public:
		StatelessWriter(GUID_t a_guid, ReliabilityKind_t reliability, Duration_t a_resend_data_period);
		StatelessWriter(Duration_t a_resend_data_period);
		~StatelessWriter();

		void	reader_locator_add(Locator_t a_locator);
		void	reader_locator_remove(Locator_t a_locator);

		void	unsent_changes_reset(void);

		CacheChange*		new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle);

	public:
		static void*	processing(void* arg);
		void			start();

	private:
		void*	_get_transceiver();
	};
}

#endif
