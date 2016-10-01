#ifndef SUBSCRIBERLISTENER_H_
#define SUBSCRIBERLISTENER_H_

#include "../../Status/Listener.h"
#include "DataReaderListener.h"

namespace CNU_DDS
{
	class SubscriberListener : public DataReaderListener
	{
	public:
		virtual void	on_data_on_readers(pSubscriber the_subscriber)	= 0;
	};
}

#endif
