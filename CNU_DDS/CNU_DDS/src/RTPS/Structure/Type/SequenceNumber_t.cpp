#include "../../../../include/RTPS/Structure/Type/SequenceNumber_t.h"

namespace CNU_DDS
{
	SequenceNumber_t::SequenceNumber_t()
	{
		high	= 0;
		low		= 0;
	}

	SequenceNumber_t::SequenceNumber_t(long a_high, unsigned long a_low)
	{
		high	= a_high;
		low		= a_low;
	}

	SequenceNumber_t	SequenceNumber_t::operator	- (const SequenceNumber_t& arg)
	{
		SequenceNumber_t	result;

		result.high	= high	- arg.high;

		if(arg.low > low)
		{
			result.high--;
			result.low	=	(0xFFFFFFFF - arg.low) + low;
		}
		else
		{
			result.low	= low	- arg.low;
		}

		return result;
	}

	SequenceNumber_t	SequenceNumber_t::operator	+ (const SequenceNumber_t& arg)
	{
		SequenceNumber_t	result	= SequenceNumber_t();

		result.high	= high;
		result.low	= low;

		if( (this->low + arg.low) < this->low )
		{
			result.high++;
		}

		result.low	= this->low + arg.low;

		return	result;
	}

	void				SequenceNumber_t::operator	+=(const SequenceNumber_t& arg)
	{
		if(this->low + arg.low < this->low)
		{
			this->high++;
		}
		this->high	+= arg.high;
		this->low	+= arg.low;
	}

	void	SequenceNumber_t::operator	= (const SequenceNumber_t& arg)
	{
		high	= arg.high;
		low		= arg.low;
	}

	bool	SequenceNumber_t::operator	==(const SequenceNumber_t& arg)
	{
		if( (this->high == arg.high) && (this->low == arg.low) )
		{
			return true;
		}

		return false;
	}

	bool	SequenceNumber_t::operator	<=(const SequenceNumber_t& arg)
	{
		if( this->high < arg.high )
		{
			return true;
		}
		else if( (this->high == arg.high) && (this->low <= arg.low) )
		{
			return true;
		}

		return false;
	}

	bool	SequenceNumber_t::operator	>=(const SequenceNumber_t& arg)
	{
		if( this->high > arg.high )
		{
			return true;
		}
		else if( (this->high == arg.high) && (this->low >= arg.low) )
		{
			return true;
		}

		return false;
	}

	bool	SequenceNumber_t::operator	<(const SequenceNumber_t& arg)
	{
		if( this->high < arg.high )
		{
			return true;
		}
		else if( (this->high == arg.high) && (this->low < arg.low) )
		{
			return true;
		}

		return false;
	}
}
