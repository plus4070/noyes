#ifndef GUIDPREFIX_T_H_
#define GUIDPREFIX_T_H_

namespace CNU_DDS
{
	class GuidPrefix_t
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

#endif
