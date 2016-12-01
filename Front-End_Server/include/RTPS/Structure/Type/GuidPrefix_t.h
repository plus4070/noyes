#ifndef GUIDPREFIX_T_H_
#define GUIDPREFIX_T_H_

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
	class CNU_DDS_DLL_API GuidPrefix_t
	{
	public:
		char	host_id[4];
		char	app_id[4];
		char	counter[4];

	public:
		void	operator	= (const GuidPrefix_t& arg);
		bool	operator	== (const GuidPrefix_t& arg);
	};

	const GuidPrefix_t	GUIDPREFIX_UNKNOWN	= {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}};
}

#ifdef __cplusplus
}
#endif
#endif
