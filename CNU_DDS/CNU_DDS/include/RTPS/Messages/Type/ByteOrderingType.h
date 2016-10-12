#ifndef	BYTE_ORDERING_TYPE_H_
#define BYTE_ORDERING_TYPE_H_

namespace CNU_DDS
{
	typedef	unsigned char	ByteOrderingType;

	const	ByteOrderingType	BYTE_ORDERING_TYPE_BIG_ENDIAN		= 0x01;
	const	ByteOrderingType	BYTE_ORDERING_TYPE_LITTLE_ENDIAN	= 0x10;

	const	ByteOrderingType	BYTE_ORDERING_TYPE_BIG_ENDIAN_LIST		= 0x04;
	const	ByteOrderingType	BYTE_ORDERING_TYPE_LITTLE_ENDIAN_LIST	= 0x40;
}

#endif
