#include "../../../include/RTPS/Structure/Reader.h"

namespace CNU_DDS
{
	Reader::Reader()
	{
		/* Working version */
		/*
		heartbeat_response_delay.sec 		= 1;
		heartbeat_response_delay.nanosec	= 0;

		heartbeat_suppression_duration.sec		= 0;
		heartbeat_suppression_duration.nanosec	= 0;
		*/

		/* Spec version */
		heartbeat_response_delay.sec 		= 0;
		heartbeat_response_delay.nanosec	= 500 * 1000 * 1000;

		heartbeat_suppression_duration.sec		= 0;
		heartbeat_suppression_duration.nanosec	= 0;
		
		/* Test version */
		/*
		heartbeat_response_delay.sec			= 0;
		heartbeat_response_delay.nanosec		= 500 * 1000 * 1000;	// 500 milliseconds

		heartbeat_suppression_duration.sec		= 0;
		heartbeat_suppression_duration.nanosec	= 0;
		*/

		reader_cache	= new HistoryCache();
	}

	Reader::~Reader()
	{
		delete(reader_cache);
	}
}
