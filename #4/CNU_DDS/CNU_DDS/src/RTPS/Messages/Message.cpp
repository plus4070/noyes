#include "../../../include/RTPS/Messages/Message.h"
#include "../../../include/RTPS/Messages/Submessages/submessages.h"
#include "../../../include/Utility/SystemInformation.h"
#include <iostream>

namespace CNU_DDS
{
	Message::Message()
	{
		submessages	= new SubmessageSeq();
	}

	Message::~Message()
	{
		submessages->deleteAll();
		delete(submessages);
	}

	ByteStream*	Message::Serialize()
	{
		SystemInformation*	si					= SystemInformation::getSystemInformationInstance();
		ByteStream*			serialized_stream	= new ByteStream();
		ByteStream*			sub_stream			= NULL;

		serialized_stream->setData(si->getSystemByteOrderingType(), (unsigned char*)&header, sizeof(Header));

		for(int i=0; i<submessages->getSize(); i++)
		{
			sub_stream	= submessages->getNodeByIndex(i+1)->value->Serialize();
			serialized_stream->attachInRear(sub_stream->getData(), sub_stream->getLength());
			delete(sub_stream);
			sub_stream	= NULL;
		}

		return serialized_stream;
	}

	bool		Message::Deserialize(ByteStream*	data)
	{
		unsigned char*	recv_stream		= data->getData();
		unsigned long	data_index		= 0;
		unsigned long	max_size		= data->getLength();
		Submessage*		recv_submessage	= NULL;

		if((recv_stream == NULL) || (max_size < sizeof(Header)) || (max_size > ByteStream::DATA_LENGTH_MAX))
		{
			return false;
		}

		memcpy(&header, recv_stream + data_index, sizeof(Header));
		data_index	+= sizeof(Header);


		if(header.protocol == PROTOCOL_RTPS)
		{
			while(max_size > data_index)
			{
				switch((SubmessageKind)recv_stream[data_index])
				{
				case SUBMESSAGE_KIND_ACKNACK:
					recv_submessage	= new AckNack();
					break;
				case SUBMESSAGE_KIND_DATA:
					recv_submessage	= new Data();
					break;
				case SUBMESSAGE_KIND_DATA_FRAG:
					recv_submessage	= new DataFrag();
					break;
				case SUBMESSAGE_KIND_GAP:
					recv_submessage	= new Gap();
					break;
				case SUBMESSAGE_KIND_HEARTBEAT:
					recv_submessage	= new Heartbeat();
					break;
				case SUBMESSAGE_KIND_HEARTBEAT_FRAG:
					recv_submessage	= new HeartbeatFrag();
					break;
				case SUBMESSAGE_KIND_INFO_DESTINATION:
					recv_submessage	= new InfoDestination();
					break;
				case SUBMESSAGE_KIND_INFO_REPLY:
					recv_submessage	= new InfoReply();
					break;
				case SUBMESSAGE_KIND_INFO_REPLY_IP4:
					break;
				case SUBMESSAGE_KIND_INFO_SOURRCE:
					recv_submessage	= new InfoSource();
					break;
				case SUBMESSAGE_KIND_INFO_TIMESTAMP:
					recv_submessage	= new InfoTimestamp();
					break;
				case SUBMESSAGE_KIND_NACK_FRAG:
					recv_submessage	= new NackFrag();
					break;
				case SUBMESSAGE_KIND_PAD:
					recv_submessage	= new Pad();
					break;
				default:
					return false;
				}

				if(recv_submessage)
				{
					recv_submessage->Deserialize(recv_stream + data_index);
					data_index	+= recv_submessage->submessage_header.submessage_length + sizeof(SubmessageHeader);
					submessages->insertInRear(recv_submessage);

					if(recv_submessage->submessage_header.submessage_length == 0)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}
}
