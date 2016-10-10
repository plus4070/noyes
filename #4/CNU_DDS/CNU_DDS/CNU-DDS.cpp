#include <iostream>
#include "include/Utility/SystemInformation.h"
#include "include/Utility/Network/NetworkManager.h"
#include "include/Utility/PlatformOrientedInterface.h"

#include "application/MessageTest.h"
#include "application/TimerTest.h"
#include "application/SocketTest.h"
#include "application/ListTest.h"
#include "application/BehaviorTest.h"
#include "application/DiscoveredDataTest.h"
#include "application/SharedPointerTest.h"

#include "application/DCPS/DCPSTest.h"
#include "application/DCPS/ThroughputTest.h"
#include "application/DCPS/ShapeTest.h"

#include "include/Utility/Network/SimpleLinkedList.h"
#include "include/Utility/Monitoring/Monitor.h"

#include "TNSNServer.h"

using namespace CNU_DDS;

int main()
{
	Monitor*	mon	= Monitor::getMonitorInstance();
	start_local_time();

	//SubmessageTest();
	//TimerTest();
	//SocketTest();
	//MessageTest();
	//MultiMessagePerformanceTest();
	//TransceiverTest();
	//ChangeForReaderListTest_OverInsert();
	//BehaviorTest();
	//DiscoveredDataTest();
	//SharedPtrListTest();

	//DCPSTest();
	//ThroughputTest();
	//start();

	int tnsOption = 0;
	printf("[ 1 : Start Front-End Server / 2 : ThroughputTest Start\n");
	printf(" input > ");
	scanf("%d", &tnsOption);

	StartTopicNamingService(tnsOption);

	if (tnsOption == 2)
		ThroughputTest();
	//else
		//ShapeTest();

	//initalizeTNSServer();
	
	
	//ShapeTest();
	



	//ChangeForReaderListPerformanceTest();
	//ChangeForReaderListTest();
	//SharedPointerTest();

	return 0;
}
