#ifndef INCLUDE_UTILITY_SYSTEMINFORMATION_H_
#define INCLUDE_UTILITY_SYSTEMINFORMATION_H_

#include "../RTPS/Messages/Type/ByteOrderingType.h"


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
	class CNU_DDS_DLL_API SystemInformation
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

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_UTILITY_SYSTEMINFORMATION_H_ */
