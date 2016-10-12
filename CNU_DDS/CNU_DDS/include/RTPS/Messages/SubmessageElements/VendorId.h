#ifndef VENDORID_H_
#define VENDORID_H_

#include "SubmessageElement.h"
#include "../../Structure/Type/VendorId_t.h"

namespace CNU_DDS
{
	class VendorId : public SubmessageElement
	{
	public:
		VendorId_t	value;
	};
}

#endif
