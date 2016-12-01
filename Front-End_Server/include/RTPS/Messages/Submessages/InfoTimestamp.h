#ifndef INFOTIMESTAMP_H_
#define INFOTIMESTAMP_H_

#include "../Submessage.h"

#include "../SubmessageElements/InclusionsOfSubmessageElement.h"

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
	class CNU_DDS_DLL_API InfoTimestamp : public Submessage
	{
	public:
		Timestamp	timestamp;

	public:
		InfoTimestamp();
		~InfoTimestamp();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
