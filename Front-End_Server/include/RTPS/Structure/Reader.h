#ifndef READER_H_
#define READER_H_

#include "Endpoint.h"
#include "HistoryCache.h"

#include "Type/Duration_t.h"
#include "../../DCPS/Infrastructure/Type/BasicDefinitions.h"

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
	class CNU_DDS_DLL_API Reader : public Endpoint
	{
	private:
		bool			expect_inline_qos;

	public:
		Duration_t		heartbeat_response_delay;
		Duration_t		heartbeat_suppression_duration;
		HistoryCache*	reader_cache;
		pDataReader		related_dcps_reader;

	public:
		Reader();
		~Reader();
	};
}

#ifdef __cplusplus
}
#endif
#endif
