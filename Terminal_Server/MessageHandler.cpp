#include "MessageHandler.h"

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void				MessageHandler::setPDDNode(PDD_NODE *PN) {
	memset(PN, 0, sizeof(PDD_NODE));
}


void				MessageHandler::addDataToNode(PPDD_NODE PN, PDD_DATA PD) {
	memcpy(&(PN->PDD_DATA), &PD, sizeof(PDD_DATA));
}

bool				MessageHandler::isPacketAvailable(PDD_NODE *PN) {
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

void			MessageHandler::setMessageTypeTopicNotExist(PDD_NODE *PN) {
	PN->PDD_HEADER.MESSAGE_TYPE = MESSAGE_TYPE_NOTEXIST;
}

void			MessageHandler::setMessageTypeProcessDone(PDD_NODE *PN) {
	PN->PDD_HEADER.MESSAGE_TYPE += MESSAGE_OPTION_PLUS_DONE;
}

void			MessageHandler::setMessageType(PDD_NODE *PN, int type) {
	PN->PDD_HEADER.MESSAGE_TYPE = type;
}

void			MessageHandler::copyDatagram(PDD_NODE * oldNode, PDD_NODE * newNode) {
	memcpy(newNode, oldNode, sizeof(PDD_NODE));
}
