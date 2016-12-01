#ifndef COUNT_T_H_
#define COUNT_T_H_

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
	class CNU_DDS_DLL_API Count_t
	{
	public:
		long	value;
	};
}

#ifdef __cplusplus
}
#endif
#endif
