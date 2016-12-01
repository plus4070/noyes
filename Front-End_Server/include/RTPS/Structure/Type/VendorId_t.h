#ifndef VENDORID_T_H_
#define VENDORID_T_H_

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
	class CNU_DDS_DLL_API VendorId_t
	{
	public:
		char	vendor_id[2];
	};

	const VendorId_t	VENDORID_UNKNOWN	= {0, 0};
}

#ifdef __cplusplus
}
#endif
#endif
