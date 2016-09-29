#include "stdafx.h"

#pragma once
class MessageHandler
{
public:
	MessageHandler();
	~MessageHandler();

	//recive data process
	void			setPDDNode(PDD_NODE *PN);
	void			addDataToNode(PPDD_NODE PN, PDD_DATA PD);

	bool			isPacketAvailable(PDD_NODE *PN);
	void			setParticipantNumber(PDD_NODE *PN, int num);
	void			setMessageTypeTopicNotExist(PDD_NODE *PN);
	void			setMessageTypeProcessDone(PDD_NODE *PN);
	void			setMessageType(PDD_NODE *PN, int type);
	void			copyDatagram(PDD_NODE * oldNode, PDD_NODE * newNode);
};

