#include "StdAfx.h"
#include "DDSMonitoring.h"

#include <stdio.h>
#include <WinSock2.h>
#include "TNSNController.h"
#include "ThreadManager.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "RTPS/Discovery/SEDPdiscoveredPublicationData.h"
#include "RTPS/Discovery/SEDPdiscoveredSubscriptionData.h"
#include "Utility/Monitoring/Monitor.h"

#include "DCPS/Domain/DomainParticipant.h"
#include "DCPS/Publication/Publisher.h"
#include "DCPS/Publication/DataWriter.h"
#include "DCPS/Subscription/Subscriber.h"
#include "DCPS/Subscription/DataReader.h"
#include "DCPS/Topic/Topic.h"

using namespace CNU_DDS;

void	static	InputMonitoringData(MonitoringData * data, int type, TNSTable * table) {
	char ip[16];
	sprintf(ip, "%d.%d.%d.%d",(unsigned char)data->locator->address[12], (unsigned char)data->locator->address[13], (unsigned char)data->locator->address[14], (unsigned char)data->locator->address[15]);
	printf("%s", ip);
	if (type == NODE_TYPE_PUB) {
		SEDPdiscoveredPublicationData	* _data = new SEDPdiscoveredPublicationData();
		_data->Deserialize(data->data);
		printf("LENGTH OF PUB DATA : %d\n", data->data->length);
		table->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, data->data->getData(), data->data->getByteOrderingType(), data->data->getLength(), NODE_TYPE_PUB, MESSAGE_TYPE_SAVE);
	} else {
		SEDPdiscoveredSubscriptionData	* _data = new SEDPdiscoveredSubscriptionData();
		_data->Deserialize(data->data);
		printf("LENGTH OF SUB DATA : %d\n", data->data->length);
		table->AddEntry((char *)_data->topic_name.c_str(), (char *)_data->type_name.c_str(), "DDS_1", "127.0.0.1", ip, data->locator->port, data->data->getData(), data->data->getByteOrderingType(), data->data->getLength(), NODE_TYPE_SUB, MESSAGE_TYPE_SAVE);
	}
}


static			UINT WINAPI MonitoringPublicationData(LPVOID p){
	ThreadManager * tm = (ThreadManager *) p;
	TNSTable	  *	table = tm->GetTNSTable();
	
	int								writer_counter = 0;
	Monitor							*monitor = Monitor::getMonitorInstance();
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredPublicationData	*pub_data = new SEDPdiscoveredPublicationData();

	while(true){
		while (monitoring_data = monitor->popWriterData()) {
			pub_data->Deserialize(monitoring_data->data);
			
			pub_data->topic_name;
			pub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			writer_counter++;

			std::cout << "[W]===================================================================" << endl;
			std::cout << "TOPIC NAME		:	" << pub_data->topic_name << endl;
			std::cout << "TYPE  NAME		:	" << pub_data->type_name << endl;
			std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;			
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
			InputMonitoringData(monitoring_data, NODE_TYPE_PUB, table);
			printf("[# of discovered writer] %d\n", writer_counter);
		}
		
		tm->Resume(THREAD_FRONTEND_SERVICE);
		monitor->waitForWriterData();
	}
}

static			UINT WINAPI MonitoringSubscriptionData(LPVOID p){
	ThreadManager * tm = (ThreadManager *) p;
	TNSTable	  *	table = tm->GetTNSTable();
	
	int								reader_counter = 0;
	Monitor							*monitor = Monitor::getMonitorInstance();
	MonitoringData					*monitoring_data = NULL;
	SEDPdiscoveredSubscriptionData	*sub_data = new SEDPdiscoveredSubscriptionData();

	while(true){
		while (monitoring_data = monitor->popReaderData())	{
			sub_data->Deserialize(monitoring_data->data);
			
			sub_data->topic_name;
			sub_data->type_name;

			monitoring_data->locator->address;
			monitoring_data->locator->port;

			reader_counter++;

			std::cout << "[R]===================================================================" << endl;
			std::cout << "TOPIC NAME		:	" << sub_data->topic_name << endl;
			std::cout << "TYPE  NAME		:	" << sub_data->type_name << endl;
			std::cout << "LOCATER ADDR		:	" << monitoring_data->locator->address << endl;
			std::cout << "LOCATER PORT		:	" << monitoring_data->locator->port << endl;
			std::cout << "======================================================================" << endl;
			InputMonitoringData(monitoring_data, NODE_TYPE_SUB, table);
			printf("[# of discovered writer] %d\n", reader_counter);
		}

		tm->Resume(THREAD_FRONTEND_SERVICE);
		monitor->waitForReaderData();
	}
}

DDSMonitoring::DDSMonitoring(ThreadManager * TM)
{
	this->tm = TM;
}


DDSMonitoring::~DDSMonitoring(void)
{
	void startMonitoring();
	void closeMonitoring();
}


void DDSMonitoring::startMonitoring(){
	this->pubMonitor = (HANDLE)_beginthreadex(NULL, 0, MonitoringPublicationData, (LPVOID)this->tm, 0, NULL);
	this->tm->SetThread(pubMonitor, THREAD_PUBLICATION_MONITORING);

	this->subMonitor = (HANDLE)_beginthreadex(NULL, 0, MonitoringSubscriptionData, (LPVOID)this->tm, 0, NULL);
	this->tm->SetThread(subMonitor, THREAD_SUBSCRIPTION_MONITORING);
}

void DDSMonitoring::closeMonitoring(){
	
}