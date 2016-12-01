#ifndef BYTESTREAM_H_
#define BYTESTREAM_H_

#include "./ByteOrderingType.h"

#ifdef CNU_DDS_DLL
#define CNU_DDS_DLL_API	__declspec(dllexport)	
#else
#define CNU_DDS_DLL_API	__declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
namespace	CNU_DDS
{
	class CNU_DDS_DLL_API	ByteStream
	{
	public:
		//static const unsigned long	DATA_LENGTH_MAX	= 2048;
		//static const unsigned long	DATA_LENGTH_MAX	= 2048;
		//static const unsigned long	DATA_LENGTH_MAX	= 1024;
		static const unsigned long	DATA_LENGTH_MAX	= 512;

	//private:
	public:
		ByteOrderingType	ordering;
		unsigned long		length;
		unsigned char		data[DATA_LENGTH_MAX];

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

	class UserStream
	{
	public:
		//static const unsigned long	USER_DATA_LENGTH_MAX	= 128;
		static const unsigned long	USER_DATA_LENGTH_MAX	= 16;

	public:
		ByteOrderingType	ordering;
		unsigned long		length;
		unsigned char		data[USER_DATA_LENGTH_MAX];		
	};
}

#ifdef __cplusplus
}
#endif
#endif