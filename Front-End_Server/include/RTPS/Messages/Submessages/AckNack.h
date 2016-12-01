#ifndef ACKNACK_H_
#define ACKNACK_H_

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
	class CNU_DDS_DLL_API AckNack : public Submessage
	{
	public:
		EntityId			reader_id;
		EntityId			writer_id;
		SequenceNumberSet	reader_sn_state;
		Count				count;

	public:
		AckNack();
		~AckNack();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
