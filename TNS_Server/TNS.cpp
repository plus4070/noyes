#include "TNS.h"

int main() {
	// 서버 인스턴트
	TCPSocket server;

	// 서버시작
	server.StartServer();

	return 0;
}
/*

int main() {
	
	RequestTable * RT = new RequestTable();


	RT->addEntry(0, 0, "000");

	RequestTable::PR_NODE PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);


	RT->addEntry(0, 0, "000");
	RT->addEntry(1, 1, "111");
	RT->addEntry(2, 2, "222");
	RT->addEntry(3, 3, "333");
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);

	
	RT->addEntry(4, 4, "444");
	RT->addEntry(5, 5, "555");
	RT->addEntry(6, 6, "666");
	RT->addEntry(7, 7, "777");

	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	

	RT->addEntry(0, 0, "000");
	RT->addEntry(1, 1, "111");
	RT->addEntry(2, 2, "222");

	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);
	PN = RT->getLastEntry();
	printf("%d %d %s\n", PN->key.REQUEST_ID, PN->key.REQUEST_SOCEKTNUMBER, PN->key.REQUEST_DATA);


	// 서버 인스턴트
	TCPSocket server;

	// 서버시작
	server.StartServer();

	return 0;
}

*/