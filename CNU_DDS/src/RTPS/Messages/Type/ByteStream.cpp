#include "../../../../include/RTPS/Messages/Type/ByteStream.h"
#include <memory.h>
#include <stdlib.h>

namespace CNU_DDS
{
	ByteStream::ByteStream()
	{
		length	= 0;
	}

	ByteStream::~ByteStream()
	{
	}

	void ByteStream::setData(ByteOrderingType ordering_type, unsigned char* input_data, unsigned long input_length)
	{
		if(input_length <= DATA_LENGTH_MAX)
		{
			length		= input_length;
			ordering	= ordering_type;

			memcpy(data, input_data, input_length);
		}
	}

	void			ByteStream::attachInFront(unsigned char* input_data, unsigned long input_length)
	{
		unsigned char	tmp[DATA_LENGTH_MAX]	= {0, };

		if(length + input_length <= DATA_LENGTH_MAX)
		{
			memcpy(tmp, data, length);
			memcpy(data, input_data, input_length);
			memcpy(data + input_length, tmp, length);

			length	+= input_length;
		}
	}

	void			ByteStream::attachInRear(unsigned char* input_data, unsigned long input_length)
	{
		if(length + input_length <= DATA_LENGTH_MAX)
		{
			memcpy(data + length, input_data, input_length);

			length	+= input_length;
		}
	}

	unsigned char* ByteStream::getData()
	{
		return data;
	}

	unsigned long	ByteStream::getLength()
	{
		return length;
	}

	ByteOrderingType	ByteStream::getByteOrderingType()
	{
		return	ordering;
	}

	void		ByteStream::ChangeByteOrdering(void* value, unsigned long length_of_value)
	{
		unsigned char	tmp;

		for(int i=0; i<(length_of_value/2); i++)
		{
			tmp												= ((unsigned char*)value)[i];
			((unsigned char*)value)[i]						= ((unsigned char*)value)[length_of_value-i-1];
			((unsigned char*)value)[length_of_value-i-1]	= tmp;
		}
	}

	void			ByteStream::operator	= (const ByteStream&	arg)
	{
		memcpy(this, &arg, sizeof(ByteStream));
	}
}
