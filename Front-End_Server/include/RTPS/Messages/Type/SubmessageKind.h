#ifndef SUBMESSAGEKIND_H_
#define SUBMESSAGEKIND_H_

namespace CNU_DDS
{
	typedef unsigned char	SubmessageKind;

	const SubmessageKind	SUBMESSAGE_KIND_PAD					= 0x01;
	const SubmessageKind	SUBMESSAGE_KIND_ACKNACK				= 0x06;
	const SubmessageKind	SUBMESSAGE_KIND_HEARTBEAT			= 0x07;
	const SubmessageKind	SUBMESSAGE_KIND_GAP					= 0x08;
	const SubmessageKind	SUBMESSAGE_KIND_INFO_TIMESTAMP		= 0x09;
	const SubmessageKind	SUBMESSAGE_KIND_INFO_SOURRCE		= 0x0c;
	const SubmessageKind	SUBMESSAGE_KIND_INFO_REPLY_IP4		= 0x0d;
	const SubmessageKind	SUBMESSAGE_KIND_INFO_DESTINATION	= 0x0e;
	const SubmessageKind	SUBMESSAGE_KIND_INFO_REPLY			= 0x0f;
	const SubmessageKind	SUBMESSAGE_KIND_NACK_FRAG			= 0x12;
	const SubmessageKind	SUBMESSAGE_KIND_HEARTBEAT_FRAG		= 0x13;
	const SubmessageKind	SUBMESSAGE_KIND_DATA				= 0x15;
	const SubmessageKind	SUBMESSAGE_KIND_DATA_FRAG			= 0x16;
}

#endif
