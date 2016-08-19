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

#include "DataWriterTest.h"
#include "DataReaderTest.h"
#include "ReceivingTest.h"

using namespace CNU_DDS;

int main()
{
	start_local_time();

	//SubmessageTest();
	//TimerTest();
	//SocketTest();
	//MessageTest();
	//TransceiverTest();
	//ChangeForReaderListTest_OverInsert();
	//BehaviorTest();
	//DiscoveredDataTest();
	//SharedPtrListTest();

	//DCPSTest();
	//ThroughputTest();
	//ShapeTest();
	//ChangeForReaderListPerformanceTest();
	//ChangeForReaderListTest();
	//SharedPointerTest();

	//dataReaderTest();
	//dataWriterTest();

	StartReceving();

	return 0;
}
