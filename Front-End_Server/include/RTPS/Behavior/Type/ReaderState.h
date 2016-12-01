#ifndef INCLUDE_RTPS_BEHAVIOR_TYPE_READERSTATE_H_
#define INCLUDE_RTPS_BEHAVIOR_TYPE_READERSTATE_H_

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
	enum ReaderState
	{
		READER_STATE_WAITING,		// 0
		READER_STATE_MAY_SEND_ACK,	// 1
		READER_STATE_MUST_SEND_ACK,	// 2
		READER_STATE_READY,			// 3
		READER_STATE_UNKNOWN		// 4
	};

	enum ReaderEvent
	{
		DATA_IS_RECEIVED,				// 0
		HEARTBEAT_IS_RECEIVED,			// 1
		GAP_IS_RECEIVED,				// 2
		MISSING_CHANGES_TRUE,			// 3
		MISSING_CHANGES_FALSE,			// 4
		AFTER_HEARTBEAT_RESONSE_DELAY,	// 5
		DELETE_WRITER_PROXY,			// 6
		SEND_A_MESSAGE_TO_WRITER		// 7
	};
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_RTPS_BEHAVIOR_TYPE_READERSTATE_H_ */
