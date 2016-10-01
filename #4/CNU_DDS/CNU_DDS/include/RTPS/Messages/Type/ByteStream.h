#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include "./ByteOrderingType.h"

namespace	CNU_DDS
{
	class ByteStream
	{
	public:
		static const unsigned long	DATA_LENGTH_MAX	= 2048;
		//static const unsigned long	DATA_LENGTH_MAX	= 2048;
		//static const unsigned long	DATA_LENGTH_MAX	= 1024;
		//static const unsigned long	DATA_LENGTH_MAX	= 512;

	//private:
	public:
		unsigned char		data[DATA_LENGTH_MAX];
		unsigned long		length;
		ByteOrderingType	ordering;

	public:
		ByteStream();
		~ByteStream();

		void			setData(ByteOrderingType ordering_type, unsigned char* input_data, unsigned long input_length);
		void			attachInFront(unsigned char* input_data, unsigned long input_length);
		void			attachInRear(unsigned char* input_data, unsigned long input_length);

		unsigned char*		getData();
		unsigned long		getLength();
		ByteOrderingType	getByteOrderingType();

		static void		ChangeByteOrdering(void* value, unsigned long length_of_value);

		void			operator	= (const ByteStream&	arg);
	};
}

#endif /* BYTESTREAM_H_ */
