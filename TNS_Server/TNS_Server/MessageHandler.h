#include "stdafx.h"

#pragma once

class MessageHandler
{
public:
	MessageHandler();
	~MessageHandler();

	//recive data process
	void			SetPDDNode(PDD_NODE *PN);
	void			AddDataToNode(PPDD_NODE PN, PDD_DATA PD);

	bool			IsPacketAvailable(PDD_NODE *PN);
	void			SetParticipantNumber(PDD_NODE *PN, int num);
	void			SetMessageTypeTopicNotExist(PDD_NODE *PN);
	void			SetMessageTypeProcessDone(PDD_NODE *PN);
	void			SetMessageType(PDD_NODE *PN, int type);
	void			CopyDatagram(PDD_NODE * oldNode, PDD_NODE * newNode);

};