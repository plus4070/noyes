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


static bool pairCompare(const pair<IN_ADDR, PDD_DATA> &firstElem, const pair<IN_ADDR, PDD_DATA> &secondElem) {
	return firstElem.first.S_un.S_addr < secondElem.first.S_un.S_addr;
}
/*
list<pair<IN_ADDR, PDD_DATA>>	ParticipantDataDistributor::getParticipantData() {
	//type, 수정된 리스트 Type 조사, 조사한 리스트들의 목록을 다른 타입의 노드들에게 전송하여 저장시킨다.
	PDD_NODE pPacket;
	vector<IN_ADDR> sendList;
	list<pair<IN_ADDR, PDD_DATA>> datagram;
	PDD_DATA data;

	PDOMAIN_ENTRY domain = ParticipantList->d_head;
	PTOPIC_ENTRY topic;
	PPARTICIPANT_ENTRY currentP;
	PPARTICIPANT_ENTRY oppositeP;
	char to[ADDRESS_SIZE], from[ADDRESS_SIZE];

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
								//printf("Distribute From : %s -> %s\tData:%s\t[%s\t%s\t%s]\n", from, to, currentP->TD_DATA, domain->TD_DOMAIN, topic->TD_TOPIC, "PUB");
								memcpy(data.PARTICIPANT_DATA, currentP->TD_DATA, MAX_DATA_SIZE);
								strcpy(data.PARTICIPANT_DOMAIN_ID, domain->TD_DOMAIN);
								strcpy(data.PARTICIPANT_TOPIC, topic->TD_TOPIC);
								data.PARTICIPANT_NODE_TYPE = NODE_TYPE_PUB;
								memcpy(data.PARTICIPANT_IP, from, ADDRESS_SIZE);

								datagram.push_back(make_pair(oppositeP->TD_PARTICIPANT_IP, data));
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
								//printf("Distribute From : %s -> %s\tData:%s\t[%s\t%s\t%s]\n", from, to, currentP->TD_DATA, domain->TD_DOMAIN, topic->TD_TOPIC, "SUB");
								memcpy(data.PARTICIPANT_DATA, currentP->TD_DATA, MAX_DATA_SIZE);
								strcpy(data.PARTICIPANT_DOMAIN_ID, domain->TD_DOMAIN);
								strcpy(data.PARTICIPANT_TOPIC, topic->TD_TOPIC);
								data.PARTICIPANT_NODE_TYPE = NODE_TYPE_SUB;
								memcpy(data.PARTICIPANT_IP, from, ADDRESS_SIZE);

								datagram.push_back(make_pair(oppositeP->TD_PARTICIPANT_IP, data));
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

	datagram.sort(pairCompare);

	list < pair<IN_ADDR, PDD_DATA> >::iterator it;
	for (it = datagram.begin(); it != datagram.end(); ++it) {
		strcpy(to, inet_ntoa((*it).first));
		printf("[From : %s\tTo : %s] %s\t%s\t%s\t%s\n",(*it).second.PARTICIPANT_IP, to, (*it).second.PARTICIPANT_DOMAIN_ID, (*it).second.PARTICIPANT_TOPIC, (*it).second.PARTICIPANT_DATA, (*it).second.PARTICIPANT_NODE_TYPE == NODE_TYPE_PUB ? "PUB" : "SUB");
	}

	return datagram;
}
*/