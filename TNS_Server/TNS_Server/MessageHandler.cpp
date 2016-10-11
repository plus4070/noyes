#include  "MessageHandler.h"

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::SetPDDNode(PDD_NODE *PN) 
{
	memset(PN, 0, sizeof(PDD_NODE));
}


void MessageHandler::AddDataToNode(PPDD_NODE PN, PDD_DATA PD) 
{
	memcpy(&(PN->PDD_DATA[PN->PDD_HEADER.NUMBER_OF_PARTICIPANT]), &PD, sizeof(PDD_DATA));
	PN->PDD_HEADER.NUMBER_OF_PARTICIPANT++;
}

bool MessageHandler::IsPacketAvailable(PDD_NODE *PN) 
{
	switch (PN->PDD_HEADER.MESSAGE_TYPE)
	{
	case MESSAGE_TYPE_MODIFY:
	case MESSAGE_TYPE_REMOVE:
	case MESSAGE_TYPE_SAVE:
		return true;
		break;
	default:
		return false;
		break;
	}
}

void MessageHandler::SetParticipantNumber(PDD_NODE *PN, int num) 
{
	PN->PDD_HEADER.NUMBER_OF_PARTICIPANT = num;
}

void MessageHandler::SetMessageTypeTopicNotExist(PDD_NODE *PN) 
{
	PN->PDD_HEADER.MESSAGE_TYPE += MESSAGE_OPTION_PLUS_DONE;
}

void MessageHandler::SetMessageTypeProcessDone(PDD_NODE *PN) 
{
	PN->PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
}

void MessageHandler::SetMessageType(PDD_NODE *PN, int type) 
{
	PN->PDD_HEADER.MESSAGE_TYPE = type;
}

void MessageHandler::CopyDatagram(PDD_NODE * oldNode, PDD_NODE * newNode) 
{
	memcpy(newNode, oldNode, sizeof(PDD_NODE));
}