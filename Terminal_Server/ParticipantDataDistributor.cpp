#include "ParticipantDataDistributor.h"
#include "TerminalTable.h"


ParticipantDataDistributor::ParticipantDataDistributor()
{
}


ParticipantDataDistributor::~ParticipantDataDistributor()
{
}

void		ParticipantDataDistributor::setPubSubList(TerminalTable * List) {
	this->ParticipantList = List;
}

bool		ParticipantDataDistributor::checkModifyTableEntry() {
	return ParticipantList->isTableModified();
}

vector<IN_ADDR>	ParticipantDataDistributor::getParticipantData(PPDD_NODE PN, int type) {
	//type, 수정된 리스트 Type 조사, 조사한 리스트들의 목록을 다른 타입의 노드들에게 전송하여 저장시킨다.
	PDD_NODE pPacket;
	vector<IN_ADDR> sendList;

	char to[ADDRESS_SIZE], from[ADDRESS_SIZE];

	PDOMAIN_ENTRY domain = ParticipantList->d_head;
	PTOPIC_ENTRY topic;
	PPARTICIPANT_ENTRY currentP;
	PPARTICIPANT_ENTRY oppositeP;

	while (domain != NULL) {
		if (domain->TD_CHANGE_FLAG) {
			topic = domain->TOPIC;

			while (topic != NULL) {
				if (topic->TD_CHANGE_FLAG_OF_PUB) {
					currentP = topic->PUB_PARTICIPANT_LIST;
					oppositeP = topic->SUB_PARTICIPANT_LIST;

					while (currentP != NULL) {
						if (currentP->TD_CHANGE_FLAG) {
							strcpy(from, inet_ntoa(currentP->TD_PARTICIPANT_IP));
							while (oppositeP != NULL) {
								strcpy(to, inet_ntoa(oppositeP->TD_PARTICIPANT_IP));
								printf("Distribute From : %s -> %s\tData:%s\t[%s\t%s\t%s]\n", from, to, currentP->TD_DATA, domain->TD_DOMAIN, topic->TD_TOPIC, "PUB");
								oppositeP = oppositeP->next;
							}
						}
						currentP->TD_CHANGE_FLAG = false;
						currentP = currentP->next;
					}
					topic->TD_CHANGE_FLAG_OF_PUB = false;
				}
				
				if (topic->TD_CHANGE_FLAG_OF_SUB) {
					currentP = topic->SUB_PARTICIPANT_LIST;
					oppositeP = topic->PUB_PARTICIPANT_LIST;
					while (currentP != NULL) {
						if (currentP->TD_CHANGE_FLAG) {
							strcpy(from, inet_ntoa(currentP->TD_PARTICIPANT_IP));
							while (oppositeP != NULL) {
								strcpy(to, inet_ntoa(oppositeP->TD_PARTICIPANT_IP));
								printf("Distribute From : %s -> %s\tData:%s\t[%s\t%s\t%s]\n", from, to, currentP->TD_DATA, domain->TD_DOMAIN, topic->TD_TOPIC, "SUB");
								oppositeP = oppositeP->next;
							}
						}
						currentP->TD_CHANGE_FLAG = false;
						currentP = currentP->next;
					}
					topic->TD_CHANGE_FLAG_OF_SUB = false;
				}
				topic = topic->next;
			}
		}
		domain->TD_CHANGE_FLAG = false;
		domain = domain->next;
	}
	this->ParticipantList->isModifyEntryExist = false;

	/*
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
	*/
	return sendList;
}