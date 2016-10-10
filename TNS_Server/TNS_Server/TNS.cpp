#include "TNS.h"
#include "TNSController.h"

int main() {
	// 서버 인스턴트
	TNSController server;

	// 서버시작
	server.StartTNSServer();

	return 0;
}