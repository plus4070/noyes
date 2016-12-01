#ifndef DOMAINID_T_H_
#define DOMAINID_T_H_

#include "BasicDefinitions.h"

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
	typedef	DOMAINID_TYPE_NATIVE	DomainId_t;
}

#ifdef __cplusplus
}
#endif
#endif
