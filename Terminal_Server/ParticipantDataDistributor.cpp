#include "ParticipantDataDistributor.h"
#include "TerminalTable.h"


ParticipantDataDistributor::ParticipantDataDistributor()
{
}


ParticipantDataDistributor::~ParticipantDataDistributor()
{
}

void		ParticipantDataDistributor::setPubSubList(TerminalTable * subList, TerminalTable * pubList) {
	this->PublicationList = pubList;
	this->SubscriptionList = subList;
}

bool		ParticipantDataDistributor::checkModifyTableEntry() {
	return SubscriptionList->isTableModified() || PublicationList->isTableModified();
}

vector<IN_ADDR>	ParticipantDataDistributor::getParticipantData(PPDD_NODE PN, int type) {
	//type, 수정된 리스트 Type 조사, 조사한 리스트들의 목록을 다른 타입의 노드들에게 전송하여 저장시킨다.
	PDD_NODE pPacket;
	vector<IN_ADDR> sendList;

	if (PublicationList->isTableModified() && type == NODE_TYPE_PUB) {
		//발간자 변경 리스트 조사, 전파 당할 대상 PUB
		pPacket.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = PublicationList->getAllModifiedData(pPacket.PDD_DATA);
		memcpy(pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_ID,"DDS_TEST_PUB", sizeof("DDS_TEST_PUB"));
		pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_SIZE = sizeof(pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_ID);
		pPacket.PDD_HEADER.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;

		PublicationList->getAllModifiedData(pPacket.PDD_DATA);
		sendList = SubscriptionList->getAllAddressList();
	}

	else if(SubscriptionList->isTableModified() && type == NODE_TYPE_SUB){
		//구독자 변경 리스트 조사, 전파 당할 대상은 SUB
		pPacket.PDD_HEADER.PARTICIPANT_NUMBER_OF_DATA = SubscriptionList->getAllModifiedData(pPacket.PDD_DATA);
		memcpy(pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_ID, "DDS_TEST_SUB", sizeof("DDS_TEST_PUB"));
		pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_SIZE = sizeof(pPacket.PDD_HEADER.PARTICIPANT_DOMAIN_ID);
		pPacket.PDD_HEADER.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;

		SubscriptionList->getAllModifiedData(pPacket.PDD_DATA);
		sendList = PublicationList->getAllAddressList();
	}

	memcpy(PN, (const char *)&pPacket, sizeof(pPacket));

	return sendList;
}