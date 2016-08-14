#include "FrontEndApp.h"

void main(void) {
	// 서버 인스턴트
	TCPSocket server;

	// 서버시작
	server.StartServer();
	return;
	/*
	




	/*
	SOCKET s; //소켓 디스크립터
	WSADATA wsaData;
	SOCKADDR_IN sin;


	TP = new TopicParser();

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup Failed. Error Code : %d\n", WSAGetLastError());
		//에러가 발생하면 WSAGetLastError() 함수를 통해 에러 코드를 확인할 수 있습니다.
		return;
	}

	puts(wsaData.szDescription); //사용하고 있는 윈속 버전을 보여줍니다.
	puts(wsaData.szSystemStatus); //사용하고 있는 윈속의 상태를 보여줍니다. 

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP/IP용 소켓 생성

	if (s == INVALID_SOCKET) {
		printf("소캣 생성 실패, 에러코드:%d\n", WSAGetLastError());
		WSACleanup(); //WS2_32DLL의 사용을 종료.
		return;
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1"); //접속 주소 설정
	sin.sin_port = htons(10000);      //포트 번호 설정

	if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
		printf("접속 실패, 에러 코드 = %u \n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return;
	}


	
	puts("윈속을 사용할 수 있습니다.");

	puts("222.122.84.200의 80번 포트에 접속을 성공하였습니다.");


	vector<string> x = TP->split("A/BB/CCC/DDDD/EEEEEE");
	puts("토큰 분석 결과");
	for (int i = 0; i < x.size(); i++) {
		cout << x.at(i) << endl;
		cout << x.at(i).c_str() << endl;

		if (send(s, x.at(i).c_str(), x.at(i).length(), 0) < 3) {
			printf("데이터 전송 실패, 에러 코드 = %u \n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return;
		}
	}
	puts("토큰들을 서버 프로그램에 전송하였습니다.");

	puts("접속 종료");
	
	if (closesocket(s) != 0) {
		printf("소켓 제거 실패, 에러 코드 = %u", WSAGetLastError());
		WSACleanup(); //WS2_32DLL의 사용을 종료.
		return;
	}

	if (WSACleanup() != 0) {
		printf("WSACleanup실패, 에러 코드 = %u\n", WSAGetLastError());
		return;
	}
	*/

}
