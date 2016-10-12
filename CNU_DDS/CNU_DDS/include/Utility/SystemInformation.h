#ifndef INCLUDE_UTILITY_SYSTEMINFORMATION_H_
#define INCLUDE_UTILITY_SYSTEMINFORMATION_H_

#include "../RTPS/Messages/Type/ByteOrderingType.h"


namespace CNU_DDS
{
	class SystemInformation
	{
	private:

	private:
		SystemInformation();

	public:
		~SystemInformation();

		static SystemInformation*	getSystemInformationInstance();
		ByteOrderingType			getSystemByteOrderingType();
	};

	static ByteOrderingType		SYSTEM_BYTE_ORDERING_TYPE;
	static bool					SYSTEM_IS_READY;
	static SystemInformation*	SYSTEM_INFORMATION_INSTANCE;
}

#endif /* INCLUDE_UTILITY_SYSTEMINFORMATION_H_ */
