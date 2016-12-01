#ifndef DATA_H_
#define DATA_H_

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
	class CNU_DDS_DLL_API Data : public Submessage
	{
	public:
		unsigned char	extraFlags[2];
		EntityId		reader_id;
		EntityId		writer_id;
		SequenceNumber	writer_sn;
		ParameterList	inline_qos;
		SerializedData	serialized_payload;

	public:
		Data();
		~Data();

		ByteStream*	Serialize();
		void		Deserialize(unsigned char* data);
	};
}

#ifdef __cplusplus
}
#endif
#endif
