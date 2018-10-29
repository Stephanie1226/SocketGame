//
//  main.cpp
//  sever_hanabi
//
//  Created by Chieh Wang on 2018/6/10.
//

//If using windows
#define __WINDOWS
//If using mac
#ifdef __MAC
#define SOCKET int
#include <sys/socket.h>   //connect,send,recv,setsockopt等
#include <sys/types.h>
#include <netinet/in.h>     // sockaddr_in, "man 7 ip" ,htons
#include <poll.h>          //poll,pollfd
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write
#include <netdb.h>         //gethostbyname
#include <errno.h>         //errno
#endif

#ifdef __WINDOWS
//#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
// need link with wsock32.lib
#pragma comment(lib, "wsock32.lib")
#endif

#include <stdio.h>
#include <string.h>          // memset
#include <string>
#include <iostream>
#include <time.h>

char card_color[50] = { 0 };
int card_number[50] = { 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5,1, 1, 1, 2, 2, 3, 3, 4, 4, 5 };
int card[50] = { 0 };
int player1[5], player2[5], player3[5], player4[5];
char play1_card[20], play2_card[20], play3_card[20], play4_card[20];
int point = 8, bomb = 3;
int pointer = 16;
int red[5] = { 0 }, green[5] = { 0 }, yellow[5] = { 0 }, blue[5] = { 0 }, white[5] = { 0 };//the line we put the card
int score = 0, remain = 50;
int recv1[7] = { 0 }, recv2[7] = { 0 }, recv3[7] = { 0 }, recv4[7] = { 0 };
char sbuffer_send1[61] = { 0 }, sbuffer_send2[61] = { 0 }, sbuffer_send3[61] = { 0 }, sbuffer_send4[61] = { 0 };

void option(int receive[]);
void clue(int receive_array[]);
void abandon(int receive_array[]);
void draw(int temp3[7]);


int main()
{
	//輸入並儲存IP位置
	char inputIpAddr[16] = "";
	printf("IP:");
	fgets(inputIpAddr, sizeof(inputIpAddr) / sizeof(inputIpAddr[0]), stdin);

	//玩家1與Sever連線
	//init1
#ifdef __WINDOWS
	WSADATA wsaData1 = { 0 };
	WORD wVer1 = MAKEWORD(2, 2);
	WSAStartup(wVer1, &wsaData1);
#endif
	//socket1
	SOCKET hSock1; // listne socket
	hSock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind1
	sockaddr_in saListen1;
	saListen1.sin_family = AF_INET;
	saListen1.sin_port = htons(10000);//玩家2與Sever連線之Port號為20000
	saListen1.sin_addr.s_addr = inet_addr(inputIpAddr);
	bind(hSock1, (sockaddr*)&saListen1, sizeof(sockaddr));
	//listen1
	listen(hSock1, 1);
	printf("等待玩家1連線...\n");
	//accept1
	sockaddr_in saClient1;
#ifdef __MAC
	socklen_t nSALen1 = sizeof(sockaddr);
#endif
#ifdef __WINDOWS
	int nSALen1 = sizeof(sockaddr);
#endif
	SOCKET hClient1 = accept(hSock1, (sockaddr*)&saClient1, &nSALen1);
	printf("玩家1連線成功 ... \n");

	//玩家2與Sever連線
	//init2
#ifdef __WINDOWS
	WSADATA wsaData2 = { 0 };
	WORD wVer2 = MAKEWORD(2, 2);
	WSAStartup(wVer2, &wsaData2);
#endif
	//socket2
	SOCKET hSock2; // listne socket
	hSock2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind2
	sockaddr_in saListen2;
	saListen2.sin_family = AF_INET;
	saListen2.sin_port = htons(20000);//玩家2與Sever連線之Port號為20000
	saListen2.sin_addr.s_addr = inet_addr(inputIpAddr);
	bind(hSock2, (sockaddr*)&saListen2, sizeof(sockaddr));
	//listen2
	listen(hSock2, 1);
	printf("等待玩家2連線...\n");
	//accept2
	sockaddr_in saClient2;
#ifdef __MAC
	socklen_t nSALen2 = sizeof(sockaddr);
#endif
#ifdef __WINDOWS
	int nSALen2 = sizeof(sockaddr);
#endif
	SOCKET hClient2 = accept(hSock2, (sockaddr*)&saClient2, &nSALen2);
	printf("玩家2連線成功 ... \n");

	//玩家3與Sever連線
	//init3
#ifdef __WINDOWS
	WSADATA wsaData3 = { 0 };
	WORD wVer3 = MAKEWORD(2, 2);
	WSAStartup(wVer3, &wsaData3);
#endif
	//socket3
	SOCKET hSock3; // listne socket
	hSock3 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind3
	sockaddr_in saListen3;
	saListen3.sin_family = AF_INET;
	saListen3.sin_port = htons(30000);//玩家3與Sever連線之Port號為30000
	saListen3.sin_addr.s_addr = inet_addr(inputIpAddr);
	bind(hSock3, (sockaddr*)&saListen3, sizeof(sockaddr));
	//listen3
	listen(hSock3, 1);
	printf("等待玩家3連線...\n");
	//accept3
	sockaddr_in saClient3;
#ifdef __MAC
	socklen_t nSALen3 = sizeof(sockaddr);
#endif
#ifdef __WINDOWS
	int nSALen3 = sizeof(sockaddr);
#endif
	SOCKET hClient3 = accept(hSock3, (sockaddr*)&saClient3, &nSALen3);
	printf("玩家3連線成功 ... \n");

	//玩家4與Sever連線
	//init4
#ifdef __WINDOWS
	WSADATA wsaData4 = { 0 };
	WORD wVer4 = MAKEWORD(2, 2);
	WSAStartup(wVer4, &wsaData4);
#endif
	//socket4
	SOCKET hSock4; // listne socket
	hSock4 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind4
	sockaddr_in saListen4;
	saListen4.sin_family = AF_INET;
	saListen4.sin_port = htons(40000);//玩家4與Sever連線之Port號為40000
	saListen4.sin_addr.s_addr = inet_addr(inputIpAddr);
	bind(hSock4, (sockaddr*)&saListen4, sizeof(sockaddr));
	//listen4
	listen(hSock4, 1);
	printf("等待玩家4連線...\n");
	//accept4
	sockaddr_in saClient4;
#ifdef __MAC
	socklen_t nSALen4 = sizeof(sockaddr);
#endif
#ifdef __WINDOWS
	int nSALen4 = sizeof(sockaddr);
#endif
	SOCKET hClient4 = accept(hSock4, (sockaddr*)&saClient4, &nSALen4);
	printf("玩家4連線成功 ... \n");
	printf("開始玩遊戲囉 ～ \n\n");

#ifdef __WINDOWS
	system("pause");
#endif

	//send, recv
	int m, n, p;
	char sbuffer_recv[100] = { 0 };
	char sbuffer_gate1[100] = { 0 }, sbuffer_gate2[100] = { 0 }, sbuffer_gate3[100] = { 0 }, sbuffer_gate4[100] = { 0 };

	//Game Start
	//發牌
	int c, v;
	int i = 0, j, temp, temp1;

	srand((unsigned)time(NULL));

	//牌面顏色
	for (c = 0; c < 50; c++) {
		if (0 <= c && c < 10) {
			card_color[c] = 'R';
		}
		else if (9 < c && c < 20) {
			card_color[c] = 'Y';
		}
		else if (19 < c && c < 30) {
			card_color[c] = 'B';
		}
		else if (29 < c && c < 40) {
			card_color[c] = 'G';
		}
		else {
			card_color[c] = 'W';
		}
	}

	for (i = 0; i < 50; i++) {
		card[i] = i;
	}

	for (i = 0; i < 50; i++) {
		j = rand() % 49;
		temp = card[i];
		card[i] = card[j];
		card[j] = temp;
	}
	//檢查牌面
	printf("check card:\n");
	for (i = 0; i < 50; i++) {
		temp1 = card[i];
		printf("card = %c%d  ", card_color[temp1], card_number[temp1]);
	}
	printf("\n");

	player1[4] = 1;
	player2[4] = 2;
	player3[4] = 3;
	player4[4] = 4;

	for (p = 0; p < 16; p++) {
		if (p >= 0 && p < 4) {
			player1[p] = card[p];
			printf("player1[%d] = %c%d", p + 1, card_color[player1[p]], card_number[player1[p]]);
		}
		else if (p >= 4 && p < 8) {
			player2[p % 4] = card[p];
			printf("player2[%d] = %c%d", p % 4 + 1, card_color[player2[p % 4]], card_number[player2[p % 4]]);
		}
		else if (p >= 8 && p < 12) {
			player3[p % 4] = card[p];
			printf("player3[%d] = %c%d", p % 4 + 1, card_color[player3[p % 4]], card_number[player3[p % 4]]);
		}
		else {
			player4[p % 4] = card[p];
			printf("player4[%d] = %c%d", p % 4 + 1, card_color[player4[p % 4]], card_number[player4[p % 4]]);
		}

		if (p == 3 || p == 7 || p == 11)
			printf("\n");
		printf("\n");
	}


	//送去player1的資料
	//
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play1_card[v] = player2[v];
		}
		else if (v > 4 && v < 10) {
			play1_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play1_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play1_card[v] = player1[v % 5];
		}
	}

	//送去player2的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play2_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play2_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play2_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play2_card[v] = player2[v % 5];
		}
	}

	//送去player3的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play3_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play3_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play3_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play3_card[v] = player3[v % 5];
		}
	}

	//送去player4的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play4_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play4_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play4_card[v] = player3[v % 5];
		}
		else if (v > 14 && v < 20) {
			play4_card[v] = player4[v % 5];
		}
	}

	send(hClient1, (char*)play1_card, sizeof(play1_card), 0);
	send(hClient2, (char*)play2_card, sizeof(play2_card), 0);
	send(hClient3, (char*)play3_card, sizeof(play3_card), 0);
	send(hClient4, (char*)play4_card, sizeof(play4_card), 0);

	while (1) {

		int b;

		//第一位玩家選擇動作
		sbuffer_gate1[0] = 1;
		sbuffer_gate2[0] = 2;
		sbuffer_gate3[0] = 2;
		sbuffer_gate4[0] = 2;

		send(hClient1, (char*)sbuffer_gate1, sizeof(sbuffer_gate1), 0);
		send(hClient2, (char*)sbuffer_gate2, sizeof(sbuffer_gate2), 0);
		send(hClient3, (char*)sbuffer_gate3, sizeof(sbuffer_gate3), 0);
		send(hClient4, (char*)sbuffer_gate4, sizeof(sbuffer_gate4), 0);

		if (bomb == 0 || score > 24 || remain < 1)
			break;

		m = recv(hClient1, (char*)sbuffer_recv, sizeof(sbuffer_recv), 0);

		for (b = 0; b < m; b++) {
			recv1[b] = sbuffer_recv[b];
		}
		option(recv1);

		//清空receive buffer
		for (n = 0; n < m; n++)
			sbuffer_recv[n] = 0;

		send(hClient1, (char*)sbuffer_send1, sizeof(sbuffer_send1), 0);
		send(hClient2, (char*)sbuffer_send2, sizeof(sbuffer_send2), 0);
		send(hClient3, (char*)sbuffer_send3, sizeof(sbuffer_send3), 0);
		send(hClient4, (char*)sbuffer_send4, sizeof(sbuffer_send4), 0);


		//第二位玩家選擇動作
		sbuffer_gate1[0] = 2;
		sbuffer_gate2[0] = 1;
		sbuffer_gate3[0] = 2;
		sbuffer_gate4[0] = 2;

		send(hClient1, (char*)sbuffer_gate1, sizeof(sbuffer_gate1), 0);
		send(hClient2, (char*)sbuffer_gate2, sizeof(sbuffer_gate2), 0);
		send(hClient3, (char*)sbuffer_gate3, sizeof(sbuffer_gate3), 0);
		send(hClient4, (char*)sbuffer_gate4, sizeof(sbuffer_gate4), 0);

		if (bomb == 0 || score > 24 || remain < 1)
			break;

		m = recv(hClient2, (char*)sbuffer_recv, sizeof(sbuffer_recv), 0);

		for (b = 0; b < m; b++) {
			recv2[b] = sbuffer_recv[b];
		}
		option(recv2);

		//清空receive buffer
		for (n = 0; n < m; n++)
			sbuffer_recv[n] = 0;

		send(hClient1, (char*)sbuffer_send1, sizeof(sbuffer_send1), 0);
		send(hClient2, (char*)sbuffer_send2, sizeof(sbuffer_send2), 0);
		send(hClient3, (char*)sbuffer_send3, sizeof(sbuffer_send3), 0);
		send(hClient4, (char*)sbuffer_send4, sizeof(sbuffer_send4), 0);

		//第三位玩家選擇動作
		sbuffer_gate1[0] = 2;
		sbuffer_gate2[0] = 2;
		sbuffer_gate3[0] = 1;
		sbuffer_gate4[0] = 2;

		send(hClient1, (char*)sbuffer_gate1, sizeof(sbuffer_gate1), 0);
		send(hClient2, (char*)sbuffer_gate2, sizeof(sbuffer_gate2), 0);
		send(hClient3, (char*)sbuffer_gate3, sizeof(sbuffer_gate3), 0);
		send(hClient4, (char*)sbuffer_gate4, sizeof(sbuffer_gate4), 0);

		if (bomb == 0 || score > 24 || remain < 1)
			break;

		m = recv(hClient3, (char*)sbuffer_recv, sizeof(sbuffer_recv), 0);

		for (b = 0; b < m; b++) {
			recv3[b] = sbuffer_recv[b];
		}
		option(recv3);

		//清空receive buffer
		for (n = 0; n < m; n++)
			sbuffer_recv[n] = 0;

		send(hClient1, (char*)sbuffer_send1, sizeof(sbuffer_send1), 0);
		send(hClient2, (char*)sbuffer_send2, sizeof(sbuffer_send2), 0);
		send(hClient3, (char*)sbuffer_send3, sizeof(sbuffer_send3), 0);
		send(hClient4, (char*)sbuffer_send4, sizeof(sbuffer_send4), 0);

		//第四位玩家選擇動作
		sbuffer_gate1[0] = 2;
		sbuffer_gate2[0] = 2;
		sbuffer_gate3[0] = 2;
		sbuffer_gate4[0] = 1;

		send(hClient1, (char*)sbuffer_gate1, sizeof(sbuffer_gate1), 0);
		send(hClient2, (char*)sbuffer_gate2, sizeof(sbuffer_gate2), 0);
		send(hClient3, (char*)sbuffer_gate3, sizeof(sbuffer_gate3), 0);
		send(hClient4, (char*)sbuffer_gate4, sizeof(sbuffer_gate4), 0);

		if (bomb == 0 || score > 24 || remain < 1)
			break;

		m = recv(hClient4, (char*)sbuffer_recv, sizeof(sbuffer_recv), 0);

		for (b = 0; b < m; b++) {
			recv4[b] = sbuffer_recv[b];
		}
		option(recv4);

		//清空receive buffer
		for (n = 0; n < m; n++)
			sbuffer_recv[n] = 0;

		send(hClient1, (char*)sbuffer_send1, sizeof(sbuffer_send1), 0);
		send(hClient2, (char*)sbuffer_send2, sizeof(sbuffer_send2), 0);
		send(hClient3, (char*)sbuffer_send3, sizeof(sbuffer_send3), 0);
		send(hClient4, (char*)sbuffer_send4, sizeof(sbuffer_send4), 0);
	}

	printf("\n遊戲結束\n");

#ifdef __WINDOWS
	closesocket(hSock1);
	closesocket(hSock2);
	closesocket(hSock3);
	closesocket(hSock4);
	WSACleanup();
#endif

#ifdef __MAC
	close(hSock1);
	close(hSock2);
	close(hSock3);
	close(hSock4);
#endif

#ifdef __WINDOWS
	system("PAUSE");
	return EXIT_SUCCESS;
#endif

	return 0;
}


void option(int receive[]) {

	int i = 0;
	int option = 0;
	//    int player = 0;
	//    int abdcard = 0;
	//    int drwcard = 0;

	option = receive[0];
	//檢查玩家動作
	switch (option) {
	case 1:
		int e;
		for (i = 0; i<3; i++) {
			sbuffer_send1[i + 1] = receive[i + 1];
			sbuffer_send2[i + 1] = receive[i + 1];
			sbuffer_send3[i + 1] = receive[i + 1];
			sbuffer_send4[i + 1] = receive[i + 1];
		}
		sbuffer_send1[0] = 1;
		sbuffer_send2[0] = 1;
		sbuffer_send3[0] = 1;
		sbuffer_send4[0] = 1;
		clue(receive);

		for (e = 0; e < 20; e++) {
			sbuffer_send1[e + 15] = play1_card[e];
			sbuffer_send2[e + 15] = play2_card[e];
			sbuffer_send3[e + 15] = play3_card[e];
			sbuffer_send4[e + 15] = play4_card[e];
		}

		sbuffer_send1[60] = receive[6];
		sbuffer_send2[60] = receive[6];
		sbuffer_send3[60] = receive[6];
		sbuffer_send4[60] = receive[6];

		remain = sbuffer_send1[14];

		break;

	case 2:
		int temp[7], m;

		for (m = 0; m < 7; m++)
			temp[m] = receive[m];

		for (i = 0; i < 9; i++) {
			sbuffer_send1[i] = 0;
			sbuffer_send2[i] = 0;
			sbuffer_send3[i] = 0;
			sbuffer_send4[i] = 0;
		}
		sbuffer_send1[9] = 2;
		sbuffer_send2[9] = 2;
		sbuffer_send3[9] = 2;
		sbuffer_send4[9] = 2;
		for (i = 10; i<12; i++) {
			sbuffer_send1[i] = 0;
			sbuffer_send2[i] = 0;
			sbuffer_send3[i] = 0;
			sbuffer_send4[i] = 0;
		}
		abandon(temp);

		sbuffer_send1[60] = receive[6];
		sbuffer_send2[60] = receive[6];
		sbuffer_send3[60] = receive[6];
		sbuffer_send4[60] = receive[6];

		remain = sbuffer_send1[14];

		break;

	case 3:
		int temp3[7];

		for (m = 0; m < 7; m++)
			temp3[m] = receive[m];

		for (i = 0; i < 10; i++) {
			sbuffer_send1[i] = 0;
			sbuffer_send2[i] = 0;
			sbuffer_send3[i] = 0;
			sbuffer_send4[i] = 0;
		}
		sbuffer_send1[10] = 3;
		sbuffer_send2[10] = 3;
		sbuffer_send3[10] = 3;
		sbuffer_send4[10] = 3;

		/*
		for (i = 0; i < 4; i++) {
		printf("%d%c\n", card_number[player1[i]], card_color[player1[i]]);
		}
		printf("bomb=%d", bomb);
		//if (bomb = 0) end();
		*/

		draw(temp3);

		sbuffer_send1[60] = receive[6];
		sbuffer_send2[60] = receive[6];
		sbuffer_send3[60] = receive[6];
		sbuffer_send4[60] = receive[6];

		remain = sbuffer_send1[14];

		break;

	default:
		break;
	}
}


void clue(int receive_array[]) {
	int k, kc; //kind of clue, color of clue, number of clue
	int i, j, u, n;
	int cardnum;//card number
	int choose_card[4] = { 0 };

	n = receive_array[1];

	switch (n) {
	case 1:
		for (cardnum = 0; cardnum<4; cardnum++) {
			choose_card[cardnum] = player1[cardnum];
		};
		break;
	case 2:
		for (cardnum = 0; cardnum<4; cardnum++) {
			choose_card[cardnum] = player2[cardnum];
		};
		break;
	case 3:
		for (cardnum = 0; cardnum<4; cardnum++) {
			choose_card[cardnum] = player3[cardnum];
		};
		break;
	case 4:
		for (cardnum = 0; cardnum<4; cardnum++) {
			choose_card[cardnum] = player4[cardnum];
		};
		break;
	default:
		break;
	}

	int R[4] = { 0 }, G[4] = { 0 }, Y[4] = { 0 }, B[4] = { 0 }, W[5] = { 0 }; //the array to record different color
	int count_r = 0, count_g = 0, count_y = 0, count_b = 0, count_w = 0;//counter for array above
	int num1[4] = { 0 }, num2[4] = { 0 }, num3[4] = { 0 }, num4[4] = { 0 }, num5[4] = { 0 };//the array to record different number
	int count_1 = 0, count_2 = 0, count_3 = 0, count_4 = 0, count_5 = 0;//counter for number array

	for (i = 0; i<4; i++) {
		switch (card_color[choose_card[i]]) {
		case 'R':
			R[count_r] = i + 1;
			count_r++;
			break;
		case 'G':
			G[count_g] = i + 1;
			count_g++;
			break;
		case 'Y':
			Y[count_y] = i + 1;
			count_y++;
			break;
		case 'B':
			B[count_b] = i + 1;
			count_b++;
			break;
		case 'W':
			W[count_w] = i + 1;
			count_w++;
			break;
		default:
			break;
		}
		switch (card_number[choose_card[i]]) {
		case 1:
			num1[count_1] = i + 1;
			count_1++;
			break;
		case 2:
			num2[count_2] = i + 1;
			count_2++;
			break;
		case 3:
			num3[count_3] = i + 1;
			count_3++;
			break;
		case 4:
			num4[count_4] = i + 1;
			count_4++;
			break;
		case 5:
			num5[count_5] = i + 1;
			count_5++;
			break;
		default:
			break;
		}
	}

	k = receive_array[2];

	//k是玩家要提示什麼(color or number)
	switch (k) {
	case 1:
		//kc是要提示的顏色r, g, y, b, w
		kc = receive_array[3];
		switch (kc) {
		case 1:
			//printf("\nThere is(are) %d red card(s) in ", count_r);
			sbuffer_send1[4] = count_r;
			sbuffer_send2[4] = count_r;
			sbuffer_send3[4] = count_r;
			sbuffer_send4[4] = count_r;
			for (j = 0; j<count_r; j++) {
				//printf("%d,", R[j]);
				sbuffer_send1[j + 5] = R[j];
				sbuffer_send2[j + 5] = R[j];
				sbuffer_send3[j + 5] = R[j];
				sbuffer_send4[j + 5] = R[j];
			}
			break;

		case 2:
			//printf("\nThere is(are) %d green card(s) in ", count_g);
			sbuffer_send1[4] = count_g;
			sbuffer_send2[4] = count_g;
			sbuffer_send3[4] = count_g;
			sbuffer_send4[4] = count_g;
			for (j = 0; j<count_g; j++) {
				//printf("%d,", G[j]);
				sbuffer_send1[j + 5] = G[j];
				sbuffer_send2[j + 5] = G[j];
				sbuffer_send3[j + 5] = G[j];
				sbuffer_send4[j + 5] = G[j];
			}
			break;

		case 3:
			//printf("\nThere is(are) %d yellow card(s) in ", count_y);
			sbuffer_send1[4] = count_y;
			sbuffer_send2[4] = count_y;
			sbuffer_send3[4] = count_y;
			sbuffer_send4[4] = count_y;
			for (j = 0; j<count_y; j++) {
				//printf("%d,", Y[j]);
				sbuffer_send1[j + 5] = Y[j];
				sbuffer_send2[j + 5] = Y[j];
				sbuffer_send3[j + 5] = Y[j];
				sbuffer_send4[j + 5] = Y[j];
			}
			break;

		case 4:
			//printf("\nThere is(are) %d blue card(s) in ", count_b);
			sbuffer_send1[4] = count_b;
			sbuffer_send2[4] = count_b;
			sbuffer_send3[4] = count_b;
			sbuffer_send4[4] = count_b;
			for (j = 0; j<count_b; j++) {
				//printf("%d,", B[j]);
				sbuffer_send1[j + 5] = B[j];
				sbuffer_send2[j + 5] = B[j];
				sbuffer_send3[j + 5] = B[j];
				sbuffer_send4[j + 5] = B[j];
			}
			break;

		case 5:
			//printf("\nThere is(are) %d blue card(s) in ", count_w);
			sbuffer_send1[4] = count_w;
			sbuffer_send2[4] = count_w;
			sbuffer_send3[4] = count_w;
			sbuffer_send4[4] = count_w;
			for (j = 0; j<count_w; j++) {
				//printf("%d,", W[j]);
				sbuffer_send1[j + 5] = W[j];
				sbuffer_send2[j + 5] = W[j];
				sbuffer_send3[j + 5] = W[j];
				sbuffer_send4[j + 5] = W[j];
			}
			break;

		default:
			break;
		}
		break;

	case 2:
		//kc是要提示的數字1,2,3,4,5
		kc = receive_array[3];
		switch (kc) {
		case 1:
			//printf("\nThere is(are) %d one card(s) in ", count_1);
			sbuffer_send1[4] = count_1;
			sbuffer_send2[4] = count_1;
			sbuffer_send3[4] = count_1;
			sbuffer_send4[4] = count_1;
			for (j = 0; j<count_1; j++) {
				//printf("%d,", num1[j]);
				sbuffer_send1[j + 5] = num1[j];
				sbuffer_send2[j + 5] = num1[j];
				sbuffer_send3[j + 5] = num1[j];
				sbuffer_send4[j + 5] = num1[j];
			}
			break;

		case 2:
			//printf("\nThere is(are) %d two card(s) in ", count_2);
			sbuffer_send1[4] = count_2;
			sbuffer_send2[4] = count_2;
			sbuffer_send3[4] = count_2;
			sbuffer_send4[4] = count_2;
			for (j = 0; j<count_2; j++) {
				//printf("%d,", num2[j]);
				sbuffer_send1[j + 5] = num2[j];
				sbuffer_send2[j + 5] = num2[j];
				sbuffer_send3[j + 5] = num2[j];
				sbuffer_send4[j + 5] = num2[j];
			}
			break;

		case 3:
			//printf("\nThere is(are) %d three card(s) in ", count_3);
			sbuffer_send1[4] = count_3;
			sbuffer_send2[4] = count_3;
			sbuffer_send3[4] = count_3;
			sbuffer_send4[4] = count_3;
			for (j = 0; j<count_3; j++) {
				//printf("%d,", num3[j]);
				sbuffer_send1[j + 5] = num3[j];
				sbuffer_send2[j + 5] = num3[j];
				sbuffer_send3[j + 5] = num3[j];
				sbuffer_send4[j + 5] = num3[j];
			}
			break;

		case 4:
			//printf("\nThere is(are) %d four card(s) in ", count_4);
			sbuffer_send1[4] = count_4;
			sbuffer_send2[4] = count_4;
			sbuffer_send3[4] = count_4;
			sbuffer_send4[4] = count_4;
			for (j = 0; j<count_4; j++) {
				//printf("%d,", num4[j]);
				sbuffer_send1[j + 5] = num4[j];
				sbuffer_send2[j + 5] = num4[j];
				sbuffer_send3[j + 5] = num4[j];
				sbuffer_send4[j + 5] = num4[j];
			}
			break;

		case 5:
			//printf("\nThere is(are) %d five card(s) in ", count_5);
			sbuffer_send1[4] = count_5;
			sbuffer_send2[4] = count_5;
			sbuffer_send3[4] = count_5;
			sbuffer_send4[4] = count_5;
			for (j = 0; j<count_5; j++) {
				//printf("%d,", num5[j]);
				sbuffer_send1[j + 5] = num5[j];
				sbuffer_send2[j + 5] = num5[j];
				sbuffer_send3[j + 5] = num5[j];
				sbuffer_send4[j + 5] = num5[j];
			}
			break;

		default:
			break;
		}
		break;
	}

	point--;

	for (u = 12; u<15; u++) {
		if (u == 12) {
			sbuffer_send1[u] = bomb;
			sbuffer_send2[u] = bomb;
			sbuffer_send3[u] = bomb;
			sbuffer_send4[u] = bomb;
		}
		else if (u == 13) {
			sbuffer_send1[u] = point;
			sbuffer_send2[u] = point;
			sbuffer_send3[u] = point;
			sbuffer_send4[u] = point;
		}
		else {
			sbuffer_send1[u] = 50 - pointer;
			sbuffer_send2[u] = 50 - pointer;
			sbuffer_send3[u] = 50 - pointer;
			sbuffer_send4[u] = 50 - pointer;
		}
	}
}


void abandon(int temp[7]) {
	int abdcard, u, v, c, t;
	abdcard = temp[4] - 1;

	printf("\n");
	printf("abdcard = %d\n\n", abdcard);

	printf("player abandom = %d \n", temp[6]);

	if (temp[6] == 1) {
		player1[abdcard] = card[pointer];
		pointer++;
	}
	else if (temp[6] == 2) {
		player2[abdcard] = card[pointer];
		pointer++;
	}
	else if (temp[6] == 3) {
		player3[abdcard] = card[pointer];
		pointer++;
	}
	else if (temp[6] == 4) {
		player4[abdcard] = card[pointer];
		pointer++;
	}

	//check
	printf("\n");
	for (t = 0; t<4; t++) {
		printf("%c%d", card_color[player1[t]], card_number[player1[t]]);
	}
	printf("\n");
	for (t = 0; t<4; t++) {
		printf("%c%d", card_color[player2[t]], card_number[player2[t]]);
	}
	printf("\n");
	for (t = 0; t<4; t++) {
		printf("%c%d", card_color[player3[t]], card_number[player3[t]]);
	}
	printf("\n");
	for (t = 0; t<4; t++) {
		printf("%c%d", card_color[player4[t]], card_number[player4[t]]);
	}
	printf("\n");


	if (point < 8) {
		point++;
	}

	//更新送去player1的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play1_card[v] = player2[v];
		}
		else if (v > 4 && v < 10) {
			play1_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play1_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play1_card[v] = player1[v % 5];
		}
	}

	//更新送去player2的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play2_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play2_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play2_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play2_card[v] = player2[v % 5];
		}
	}

	//更新送去player3的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play3_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play3_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play3_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play3_card[v] = player3[v % 5];
		}
	}

	//更新送去player4的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play4_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play4_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play4_card[v] = player3[v % 5];
		}
		else if (v > 14 && v < 20) {
			play4_card[v] = player4[v % 5];
		}
	}

	for (c = 0; c < 20; c++) {
		sbuffer_send1[c + 15] = play1_card[c];
		sbuffer_send2[c + 15] = play2_card[c];
		sbuffer_send3[c + 15] = play3_card[c];
		sbuffer_send4[c + 15] = play4_card[c];
	}

	for (u = 12; u<15; u++) {
		if (u == 12) {
			sbuffer_send1[u] = bomb;
			sbuffer_send2[u] = bomb;
			sbuffer_send3[u] = bomb;
			sbuffer_send4[u] = bomb;
		}
		else if (u == 13) {
			sbuffer_send1[u] = point;
			sbuffer_send2[u] = point;
			sbuffer_send3[u] = point;
			sbuffer_send4[u] = point;
		}
		else {
			sbuffer_send1[u] = 50 - pointer;
			sbuffer_send2[u] = 50 - pointer;
			sbuffer_send3[u] = 50 - pointer;
			sbuffer_send4[u] = 50 - pointer;
		}
	}
}

void draw(int temp3[7])
{
	int put, putting = 0;//the card you put
	put = temp3[5] - 1;
	int c, v, u;

	if (temp3[6] == 1) {
		putting = player1[put];
		player1[put] = card[pointer];
		pointer++;
	}
	else if (temp3[6] == 2) {
		putting = player2[put];
		player2[put] = card[pointer];
		pointer++;
	}
	else if (temp3[6] == 3) {
		putting = player3[put];
		player3[put] = card[pointer];
		pointer++;
	}
	else if (temp3[6] == 4) {
		putting = player4[put];
		player4[put] = card[pointer];
		pointer++;
	}

	//更新送去player1的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play1_card[v] = player2[v];
		}
		else if (v > 4 && v < 10) {
			play1_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play1_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play1_card[v] = player1[v % 5];
		}
	}

	//更新送去player2的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play2_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play2_card[v] = player3[v % 5];
		}
		else if (v > 9 && v < 15) {
			play2_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play2_card[v] = player2[v % 5];
		}
	}

	//更新送去player3的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play3_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play3_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play3_card[v] = player4[v % 5];
		}
		else if (v > 14 && v < 20) {
			play3_card[v] = player3[v % 5];
		}
	}

	//更新送去player4的資料
	for (v = 0; v < 20; v++) {
		if (v >= 0 && v < 5) {
			play4_card[v] = player1[v];
		}
		else if (v > 4 && v < 10) {
			play4_card[v] = player2[v % 5];
		}
		else if (v > 9 && v < 15) {
			play4_card[v] = player3[v % 5];
		}
		else if (v > 14 && v < 20) {
			play4_card[v] = player4[v % 5];
		}
	}

	//更新的資料(player的新卡牌)送進buffer
	for (c = 0; c < 20; c++) {
		sbuffer_send1[c + 15] = play1_card[c];
		sbuffer_send2[c + 15] = play2_card[c];
		sbuffer_send3[c + 15] = play3_card[c];
		sbuffer_send4[c + 15] = play4_card[c];
	}


	if (card_number[putting] == 1) {
		switch (card_color[putting]) {
		case 'R':
			if (red[0] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[35] = 1;
				sbuffer_send2[35] = 1;
				sbuffer_send3[35] = 1;
				sbuffer_send4[35] = 1;

				red[0] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'G':
			if (green[0] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[40] = 1;
				sbuffer_send2[40] = 1;
				sbuffer_send3[40] = 1;
				sbuffer_send4[40] = 1;

				green[0] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'Y':
			if (yellow[0] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[45] = 1;
				sbuffer_send2[45] = 1;
				sbuffer_send3[45] = 1;
				sbuffer_send4[45] = 1;

				yellow[0] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'B':
			if (blue[0] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[50] = 1;
				sbuffer_send2[50] = 1;
				sbuffer_send3[50] = 1;
				sbuffer_send4[50] = 1;

				blue[0] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'W':
			if (white[0] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[55] = 1;
				sbuffer_send2[55] = 1;
				sbuffer_send3[55] = 1;
				sbuffer_send4[55] = 1;

				white[0] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		default:
			break;
		}
	}

	else if (card_number[putting] == 2) {
		switch (card_color[putting]) {
		case 'R':
			if (red[0] == 1 && red[1] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[36] = 1;
				sbuffer_send2[36] = 1;
				sbuffer_send3[36] = 1;
				sbuffer_send4[36] = 1;

				red[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;

				bomb--;
			}
			break;
		case 'G':
			if (green[0] == 1 && green[1] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[41] = 1;
				sbuffer_send2[41] = 1;
				sbuffer_send3[41] = 1;
				sbuffer_send4[41] = 1;

				green[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'Y':
			if (yellow[0] == 1 && yellow[1] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[46] = 1;
				sbuffer_send2[46] = 1;
				sbuffer_send3[46] = 1;
				sbuffer_send4[46] = 1;

				yellow[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'B':
			if (blue[0] == 1 && blue[1] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[51] = 1;
				sbuffer_send2[51] = 1;
				sbuffer_send3[51] = 1;
				sbuffer_send4[51] = 1;

				blue[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'W':
			if (white[0] == 1 && white[1] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[56] = 1;
				sbuffer_send2[56] = 1;
				sbuffer_send3[56] = 1;
				sbuffer_send4[56] = 1;

				white[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		default:
			break;
		}
	}

	else if (card_number[putting] == 3) {
		switch (card_color[putting]) {
		case 'R':
			if (red[1] == 1 && red[2] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[37] = 1;
				sbuffer_send2[37] = 1;
				sbuffer_send3[37] = 1;
				sbuffer_send4[37] = 1;

				red[2] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'G':
			if (green[1] == 1 && green[2] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[42] = 1;
				sbuffer_send2[42] = 1;
				sbuffer_send3[42] = 1;
				sbuffer_send4[42] = 1;

				green[2] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'Y':
			if (yellow[1] == 1 && yellow[2] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[47] = 1;
				sbuffer_send2[47] = 1;
				sbuffer_send3[47] = 1;
				sbuffer_send4[47] = 1;

				yellow[2] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'B':
			if (blue[1] == 1 && blue[2] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[52] = 1;
				sbuffer_send2[52] = 1;
				sbuffer_send3[52] = 1;
				sbuffer_send4[52] = 1;

				blue[2] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'W':
			if (white[1] == 1 && white[2] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[57] = 1;
				sbuffer_send2[57] = 1;
				sbuffer_send3[57] = 1;
				sbuffer_send4[57] = 1;

				white[1] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		default:
			break;
		}
	}

	else if (card_number[putting] == 4) {
		switch (card_color[putting]) {
		case 'R':
			if (red[2] == 1 && red[3] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[38] = 1;
				sbuffer_send2[38] = 1;
				sbuffer_send3[38] = 1;
				sbuffer_send4[38] = 1;

				red[3] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'G':
			if (green[2] == 1 && green[3] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[43] = 1;
				sbuffer_send2[43] = 1;
				sbuffer_send3[43] = 1;
				sbuffer_send4[43] = 1;

				green[3] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'Y':
			if (yellow[2] == 1 && yellow[3] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[48] = 1;
				sbuffer_send2[48] = 1;
				sbuffer_send3[48] = 1;
				sbuffer_send4[48] = 1;

				yellow[3] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'B':
			if (blue[2] == 1 && blue[3] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[53] = 1;
				sbuffer_send2[53] = 1;
				sbuffer_send3[53] = 1;
				sbuffer_send4[53] = 1;

				blue[3] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		case 'W':
			if (white[2] == 1 && white[3] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[58] = 1;
				sbuffer_send2[58] = 1;
				sbuffer_send3[58] = 1;
				sbuffer_send4[58] = 1;

				white[3] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			break;
		default:
			break;
		}
	}

	else if (card_number[putting] == 5) {
		switch (card_color[putting]) {
		case 'R':
			if (red[3] == 1 && red[4] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[39] = 1;
				sbuffer_send2[39] = 1;
				sbuffer_send3[39] = 1;
				sbuffer_send4[39] = 1;

				red[4] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			if (point < 8)
				point++;
			break;
		case 'G':
			if (green[3] == 1 && green[4] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[44] = 1;
				sbuffer_send2[44] = 1;
				sbuffer_send3[44] = 1;
				sbuffer_send4[44] = 1;

				green[4] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			if (point < 8)
				point++;
			break;
		case 'Y':
			if (yellow[3] == 1 && yellow[4] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[49] = 1;
				sbuffer_send2[49] = 1;
				sbuffer_send3[49] = 1;
				sbuffer_send4[49] = 1;

				yellow[4] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			if (point < 8)
				point++;
			break;
		case 'B':
			if (blue[3] == 1 && blue[4] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[54] = 1;
				sbuffer_send2[54] = 1;
				sbuffer_send3[54] = 1;
				sbuffer_send4[54] = 1;

				blue[4] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			if (point < 8)
				point++;
			break;
		case 'W':
			if (white[3] == 1 && white[4] == 0) {
				//printf("Sucess!");
				sbuffer_send1[11] = 1;
				sbuffer_send2[11] = 1;
				sbuffer_send3[11] = 1;
				sbuffer_send4[11] = 1;

				sbuffer_send1[59] = 1;
				sbuffer_send2[59] = 1;
				sbuffer_send3[59] = 1;
				sbuffer_send4[59] = 1;

				white[4] = 1;
				score++;
			}
			else {
				//printf("Failed!");
				sbuffer_send1[11] = 2;
				sbuffer_send2[11] = 2;
				sbuffer_send3[11] = 2;
				sbuffer_send4[11] = 2;
				bomb--;
			}
			if (point < 8)
				point++;
			break;
		default:
			break;
		}
	}

	for (u = 12; u < 15; u++) {
		if (u == 12) {
			sbuffer_send1[u] = bomb;
			sbuffer_send2[u] = bomb;
			sbuffer_send3[u] = bomb;
			sbuffer_send4[u] = bomb;
		}
		else if (u == 13) {
			sbuffer_send1[u] = point;
			sbuffer_send2[u] = point;
			sbuffer_send3[u] = point;
			sbuffer_send4[u] = point;
		}
		else {
			sbuffer_send1[u] = 50 - pointer;
			sbuffer_send2[u] = 50 - pointer;
			sbuffer_send3[u] = 50 - pointer;
			sbuffer_send4[u] = 50 - pointer;
		}
	}
}

