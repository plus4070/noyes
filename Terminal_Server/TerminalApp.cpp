#include "TCPSocket.h"
#include "TNSController.h"

int main() {
	// 서버 인스턴트
	TNSController server;

	// 서버시작
	server.startTNSServer();

	//TCPSocket server;
	//server.StartServer();
	return 0;
}
