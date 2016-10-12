#ifndef APPLICATION_MESSAGETEST_H_
#define APPLICATION_MESSAGETEST_H_

#include "../include/RTPS/Messages/Submessages/Submessages.h"
#include "../include/RTPS/Messages/Message.h"

namespace CNU_DDS
{
	void	MessageTest();
	void	SubmessageTest();
	void	TransceiverTest();
	void	MultiMessagePerformanceTest();

	bool	SubmessageTest_AckNack();
	bool	SubmessageTest_Data();
	bool	SubmessageTest_DataFrag();
	bool	SubmessageTest_Gap();
	bool	SubmessageTest_Heartbeat();
	bool	SubmessageTest_HeartbeatFrag();
	bool	SubmessageTest_InfoDestination();
	bool	SubmessageTest_InfoReply();
	bool	SubmessageTest_InfoSource();
	bool	SubmessageTest_InfoTimestamp();
	bool	SubmessageTest_NackFrag();
}

#endif /* APPLICATION_MESSAGETEST_H_ */
