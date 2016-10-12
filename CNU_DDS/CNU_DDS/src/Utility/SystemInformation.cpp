#include "../../include/Utility/SystemInformation.h"

namespace CNU_DDS
{
	SystemInformation::SystemInformation()
	{
		unsigned long	tmp_long	= 0x00000001;
		unsigned char*	tmp_char	= (unsigned char*)&tmp_long;

		if(*tmp_char == 0x00)
		{
			SYSTEM_BYTE_ORDERING_TYPE	= BYTE_ORDERING_TYPE_BIG_ENDIAN;
		}
		else
		{
			SYSTEM_BYTE_ORDERING_TYPE	= BYTE_ORDERING_TYPE_LITTLE_ENDIAN;
		}
	}

	SystemInformation::~SystemInformation()
	{

	}

	SystemInformation*	SystemInformation::getSystemInformationInstance()
	{
		if(SYSTEM_IS_READY == true)
		//if(SystemInformation::isExist	 == true)
		{
			// do nothing
		}
		else
		{
			//SystemInformation::instance	= new SystemInformation();
			//SystemInformation::isExist	= true;
			SYSTEM_INFORMATION_INSTANCE	= new SystemInformation();
			SYSTEM_IS_READY	= true;
		}

		return SYSTEM_INFORMATION_INSTANCE;
		//return	SystemInformation::instance;
	}

	ByteOrderingType			SystemInformation::getSystemByteOrderingType()
	{
		if(SYSTEM_IS_READY == true)
		{
			// do nothing
		}
		else
		{
			SYSTEM_INFORMATION_INSTANCE	= new SystemInformation();
			SYSTEM_IS_READY	= true;
		}

		return	SYSTEM_BYTE_ORDERING_TYPE;
	}
}

