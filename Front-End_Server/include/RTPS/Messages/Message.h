#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "Header.h"
#include "Submessage.h"

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
	class CNU_DDS_DLL_API Message
	{
	public:
		Header			header;
		SubmessageSeq*	submessages;

	public:
		Message();
		~Message();

		ByteStream*	Serialize();
		bool		Deserialize(ByteStream*	data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
