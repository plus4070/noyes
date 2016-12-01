#ifndef SERIALIZEDDATA_H_
#define SERIALIZEDDATA_H_

#include "SubmessageElement.h"
#include "../Type/ByteStream.h"

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
	class CNU_DDS_DLL_API SerializedData : public SubmessageElement
	{
	public:
		unsigned char	info[4];
		ByteStream		value;

	};

	static	unsigned char	SERIALIZED_DATA_INFO_CDR_BE[4]		=	{0x00, 0x00, 0x00, 0x00};
	static	unsigned char	SERIALIZED_DATA_INFO_CDR_LE[4]		=	{0x00, 0x01, 0x00, 0x00};
	static	unsigned char	SERIALIZED_DATA_INFO_PL_CDR_BE[4]	=	{0x00, 0x02, 0x00, 0x00};
	static	unsigned char	SERIALIZED_DATA_INFO_PL_CDR_LE[4]	=	{0x00, 0x03, 0x00, 0x00};
}

#ifdef __cplusplus
}
#endif
#endif
