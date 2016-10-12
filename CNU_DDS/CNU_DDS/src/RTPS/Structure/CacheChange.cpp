#include "../../../include/RTPS/Structure/CacheChange.h"

namespace CNU_DDS
{
	bool 		CacheChange::operator	>	(const CacheChange& arg)
	{
		if(this->sequence_number.high > arg.sequence_number.high && this->sequence_number.low > arg.sequence_number.low)
		{
			return true;
		}
		else if(this->sequence_number.high == arg.sequence_number.high && this->sequence_number.low > arg.sequence_number.low)
		{
			return true;
		}

		return false;
	}

	bool 		CacheChange::operator	<	(const CacheChange& arg)
	{
		if(this->sequence_number.high < arg.sequence_number.high && this->sequence_number.low < arg.sequence_number.low)
		{
			return true;
		}
		else if(this->sequence_number.high == arg.sequence_number.high && this->sequence_number.low < arg.sequence_number.low)
		{
			return true;
		}

		return false;
	}

	void CacheChange::operator	=	(const CacheChange& arg)
	{
		memcpy(this, &arg, sizeof(CacheChange));
	}

	bool 		CacheChange::operator	>=	(const CacheChange& arg)
	{
		if(this->sequence_number.high >= arg.sequence_number.high && this->sequence_number.low >= arg.sequence_number.low)
		{
			return true;
		}

		return false;
	}

	bool 		CacheChange::operator	<=	(const CacheChange& arg)
	{
		if(this->sequence_number.high <= arg.sequence_number.high && this->sequence_number.low <= arg.sequence_number.low)
		{
			return true;
		}

		return false;
	}

	bool 		CacheChange::operator	==	(const CacheChange& arg)
	{
		return !memcmp(this, &arg, sizeof(CacheChange)-sizeof(ByteStream*));
	}
}
