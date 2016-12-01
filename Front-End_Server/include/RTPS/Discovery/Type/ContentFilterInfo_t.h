#ifndef CONTENTFILTERINFO_T_H_
#define CONTENTFILTERINFO_T_H_

#include "FilterResult_t.h"
#include "FilterSignature_t.h"

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
	class CNU_DDS_DLL_API ContentFilterInfo_t
	{
	public:
		FilterResult_t		filter_result;
		FilterSignatureSeq	filter_signatures;
	};
}

#ifdef __cplusplus
}
#endif
#endif
