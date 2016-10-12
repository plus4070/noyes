#ifndef CONTENTFILTERINFO_T_H_
#define CONTENTFILTERINFO_T_H_

#include "FilterResult_t.h"
#include "FilterSignature_t.h"

namespace CNU_DDS
{
	class ContentFilterInfo_t
	{
	public:
		FilterResult_t		filter_result;
		FilterSignatureSeq	filter_signatures;
	};
}

#endif
