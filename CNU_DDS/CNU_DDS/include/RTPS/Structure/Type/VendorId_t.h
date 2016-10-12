#ifndef VENDORID_T_H_
#define VENDORID_T_H_

namespace CNU_DDS
{
	class VendorId_t
	{
	public:
		char	vendor_id[2];
	};

	const VendorId_t	VENDORID_UNKNOWN	= {0, 0};
}

#endif
