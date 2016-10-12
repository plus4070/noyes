#include "../../../include/RTPS/Structure/Writer.h"
#include <stdio.h>

namespace CNU_DDS
{
	Writer::Writer()
	{
		/* Working version */
		/*
		nack_response_delay.sec 	= 1;
		nack_response_delay.nanosec	= 200 * 1000 * 1000;

		nack_suppression_duration.sec		= 0;
		nack_suppression_duration.nanosec	= 0;
		*/

		/* Spec version */
		nack_response_delay.sec 			= 0;
		nack_response_delay.nanosec			= 200 * 1000 * 1000;

		nack_suppression_duration.sec		= 0;
		nack_suppression_duration.nanosec	= 0;
		
		/* Test version */
		/*
		nack_response_delay.sec				= 0;
		nack_response_delay.nanosec			= 200 * 1000 * 1000;	// 100 milliseconds

		nack_suppression_duration.sec		= 0;
		nack_suppression_duration.nanosec	= 0;

		*/
		heartbeat_period.sec		= 0;
		heartbeat_period.nanosec	= 500 * 1000 * 1000;
		//heartbeat_period.nanosec	= 300 * 1000 * 1000;
		//heartbeat_period.nanosec	= 100 * 1000 * 1000;

		last_change_sequence_number.high	= 0;
		last_change_sequence_number.low		= 0;

		writer_cache	= new HistoryCache();
	}

	Writer::~Writer()
	{
		delete(writer_cache);
	}

	CacheChange*		Writer::new_change(ChangeKind_t kind, ByteStream* data, InstanceHandle_t handle)
	{
		CacheChange*	result;

		last_change_sequence_number	+= SequenceNumber_t(0,1);

		result	= new CacheChange();
		result->kind			= kind;
		memcpy(&result->writer_guid, &this->guid, sizeof(GUID_t));
		result->sequence_number	= last_change_sequence_number;
		result->data_value		= data;
		result->instance_handle	= handle;

		writer_cache->add_change(result);

		return	result;
	}
}
