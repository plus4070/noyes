#ifndef TOPICKIND_T_H_
#define TOPICKIND_T_H_

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
	class CNU_DDS_DLL_API TopicKind_t
	{
	public:
		long	value;
	};

	const long	NO_KEY		= 1;
	const long	WITH_KEY	= 2;
}

#ifdef __cplusplus
}
#endif
#endif
