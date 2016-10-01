#ifndef WRITER_H_
#define WRITER_H_

#include "Endpoint.h"
#include "HistoryCache.h"

#include "Type/Duration_t.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"
#include "../Messages/Type/ByteStream.h"

namespace CNU_DDS
{
	class Writer : public Endpoint
	{
	public:
		bool				push_mode;
		Duration_t			heartbeat_period;
		Duration_t			nack_response_delay;
		Duration_t			nack_suppression_duration;
		SequenceNumber_t	last_change_sequence_number;
		HistoryCache*		writer_cache;
		pDataWriter			related_dcps_writer;

	public:
		Writer();
		~Writer();

		CacheChange*		new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle);
	};
}

#endif
