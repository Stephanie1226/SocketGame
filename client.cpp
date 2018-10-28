//If using windows
#define __WINDOWS
//If using mac 
#ifdef __MAC
#define SOCKET int
#include <sys/socket.h>   //connect,send,recv,setsockopt等
#include <sys/types.h>
#include <netinet/in.h>     // sockaddr_in, "man 7 ip" ,htons
#include <poll.h>             //poll,pollfd
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>        //read,write
#include <netdb.h>         //gethostbyname
#include <errno.h>         //errno
#endif

#ifdef __WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock.h>
// need link with wsock32.lib
#pragma comment(lib, "wsock32.lib")
#endif

#include <stdio.h>
#include <string.h>          // memset
#include <string>
#include <iostream>
#include <time.h>
#include<math.h>
#include<windows.h>
#include <conio.h>
#include <winsock.h>

using namespace std;

void gotoxy(int, int); //移動游標至指定位置
void Clr();
void Start_Screen(int, int);  //開始畫面
void Start_Drawing();  //畫出開始界面
void Game_Screen(int row, int column);//遊戲界面
void Game_Drawing();//畫出遊戲界面
void Game_Drawing_Half();//畫出半個遊戲界面（為了更新卡牌
void Rule_Screen(int row, int column);//規則界面
void Rule_Drawing();//畫出規則界面
void Rule();//規則界面的動作
void End_Screen(int row, int column);//結束界面
void End_Drawing();//畫出結束界面
void Connect_Screen(int row, int column);//鏈接界面
void Connect_Drawing();//畫出鏈接界面
void set_console_color(unsigned short color_index); //顏色設定
int inoutkk(int startpositiony, int startpositionx, int optionnumber, int distance);//選擇動作

char card_color[50] = { 0 };
int card_number[50] = { 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5,1, 1, 1, 2, 2, 3, 3, 4, 4, 5 };
int playerme[5], player1[5], player2[5], player3[5];
int point = 8, bomb = 3, pointer, remain = 50;
int score = 0;
int red[6] = { 0 }, green[6] = { 0 }, yellow[6] = { 0 }, blue[6] = { 0 }, white[6] = { 0 };//the line we put the card
char cbuffergate[100] = { 0 }, cbuffer_send[7] = { 0 };

int main()
{
	//init
#ifdef __WINDOWS
	WSADATA wsaData = { 0 };
	WORD wVer = MAKEWORD(2, 2);
	WSAStartup(wVer, &wsaData);
#endif

	int conn_success, port_num = 0;
	char inputIpAddr[16] = "";

	//int init;
	char kk[2];
	int x, y, lx, ly;
	int ntr;
	int entr;
	//////////////開始做開始的選擇
	Start_Drawing();
	y = 22;
	x = 66;
	gotoxy(22, 64);
	int flag = 0;
	set_console_color(0x0c);
	printf("SSSS");

	while (flag == 0) {
		while (1) {
			kk[0] = _getch();
			kk[1] = _getch();
			switch (kk[1])
			{
			case 80:
				lx = x;
				x = x + 8;
				break;
			case 77:
				lx = x;
				x = x + 8;
				break;
			case 75:
				lx = x;
				x = x - 8;
				break;
			case 72:
				lx = x;
				x = x - 8;
				break;
			default:
				break;
			}
			if (x < 66 || x > 82) {
				x = lx;
			}
			else {
				gotoxy(y, lx - 2);
				set_console_color(0x0f);
				printf("____");
				gotoxy(y, x - 2);
				set_console_color(0x0c);
				printf("SSSS");
			}
			if (kk[0] == 13) {
				break;
			}
		}
		switch (x) {
		case 66:
			flag = 1;
			break;
		case 74:
			system("cls");
			Rule();
			system("cls");
			Start_Drawing();
			continue;
		case 82:
			gotoxy(40, 5);
			set_console_color(0x0f);
			WSACleanup();
			system("pause");
			return EXIT_SUCCESS;
		default:
			break;
		}
	}
	set_console_color(0x0f);
	system("cls");

	Connect_Drawing();
	gotoxy(2, 14);
	printf("IP:");
	fgets(inputIpAddr, sizeof(inputIpAddr) / sizeof(inputIpAddr[0]), stdin);  //輸入並儲存IP位置
	gotoxy(5, 14);
	printf("Port:");
	scanf("%d", &port_num);  //輸入port號，並儲存於port_num
							 //玩家1與Sever連線之Port號為10000
							 //玩家2與Sever連線之Port號為20000
							 //玩家3與Sever連線之Port號為30000
							 //玩家4與Sever連線之Port號為40000


							 //socket
	SOCKET hServer; // listen socket
	hServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//connect
	sockaddr_in saServer;
	saServer.sin_family = AF_INET;
	saServer.sin_port = htons(port_num);
	saServer.sin_addr.s_addr = inet_addr(inputIpAddr);
	conn_success = connect(hServer, (sockaddr*)&saServer, sizeof(sockaddr));

	if (conn_success == 0) {
		printf("連線成功 ... 開始玩遊戲囉 ～ \n\n");
	}
	else {
		printf("連線失敗 ...\n");
#ifdef __WINDOWS
		WSACleanup();
		system("pause");
		return EXIT_SUCCESS;
#endif
	}

	Game_Drawing();
	//send, recv
	int n, i;
	size_t m;
	char buffer1[100] = { 0 }, cbuffer_recv[61] = { 0 };

	//牌(顏色)初始化
	int c;
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


	//從Sever端得到其他三人的牌
	m = recv(hServer, (char*)buffer1, sizeof(buffer1), 0);

	for (i = 0; i<20; i++) {
		if (i >= 0 && i < 5) {
			player1[i] = buffer1[i];
		}
		else if (i > 4 && i < 10) {
			player2[i % 5] = buffer1[i];
		}
		else if (i > 9 && i < 15) {
			player3[i % 5] = buffer1[i];
		}
		else if (i > 14 && i < 20) {
			playerme[i % 5] = buffer1[i];
		}
	}


	//清空buffer
	printf("\n");
	for (n = 0; n < m; n++) {
		buffer1[n] = 0;
	}
	printf("\n");


	//印牌
	gotoxy(22, 52);
	printf("%d", playerme[4]);

	gotoxy(18, 3);
	printf("%d", player1[4]);
	x = 6;
	y = 9;
	for (i = 0; i<4; i++) {
		gotoxy(y, x);
		switch (card_color[player1[i]]) {
		case 'R': cout << "紅色"; break;
		case 'G': cout << "綠色"; break;
		case 'Y': cout << "黃色"; break;
		case 'B': cout << "藍色"; break;
		case 'W': cout << "白色"; break;
		}
		switch (card_number[player1[i]]) {
		case 1: cout << "1  "; break;
		case 2: cout << "2  "; break;
		case 3: cout << "3  "; break;
		case 4: cout << "4  "; break;
		case 5: cout << "5  "; break;
		}
		y = y + 5;
	}

	gotoxy(1, 52);
	printf("%d", player2[4]);
	y = 3;
	x = 61;
	for (i = 0; i<4; i++) {
		gotoxy(y, x);
		switch (card_color[player2[i]]) {
		case 'R': cout << "紅色"; break;
		case 'G': cout << "綠色"; break;
		case 'Y': cout << "黃色"; break;
		case 'B': cout << "藍色"; break;
		case 'W': cout << "白色"; break;
		default: break;
		}
		switch (card_number[player2[i]]) {
		case 1: cout << "1  "; break;
		case 2: cout << "2  "; break;
		case 3: cout << "3  "; break;
		case 4: cout << "4  "; break;
		case 5: cout << "5  "; break;
		default: break;
		}
		x = x - 10;
	}

	gotoxy(18, 96);
	printf("%d", player3[4]);
	x = 86;
	y = 24;
	for (i = 0; i<4; i++) {
		gotoxy(y, x);
		switch (card_color[player3[i]]) {
		case 'R': cout << "紅色"; break;
		case 'G': cout << "綠色"; break;
		case 'Y': cout << "黃色"; break;
		case 'B': cout << "藍色"; break;
		case 'W': cout << "白色"; break;
		default: break;
		}
		switch (card_number[player3[i]]) {
		case 1: cout << "1  "; break;
		case 2: cout << "2  "; break;
		case 3: cout << "3  "; break;
		case 4: cout << "4  "; break;
		case 5: cout << "5  "; break;
		default: break;
		}
		y = y - 5;
	}
	gotoxy(4, 14);
	printf("8");
	gotoxy(5, 14);
	printf("3");
	gotoxy(6, 14);
	printf("34");
	while (1) {
		//int kk;
		int option, player, CorN, clueColor, clueNum;
		int abdcard, drwcard;
		int r, e, s;

		recv(hServer, (char*)cbuffergate, sizeof(cbuffergate), 0);

		if (bomb == 0 || score > 24 || remain < 1)
			break;

		if (cbuffergate[0] == 1) {

			ntr = 0;
			while (ntr == 0) {
				gotoxy(31, 60);
				printf("你要幹啥？");
				gotoxy(32, 60);
				printf("  提示  棄牌  出牌");
				option = inoutkk(32, 60, 3, 6);
				if (point < 1 && option == 1) {
					gotoxy(33, 60);
					printf("母湯選提示喔~  好");
					gotoxy(33, 76);
					entr = _getch();
					if (entr == 13) ntr = 0;
					gotoxy(33, 60);
					printf("                 ");
				}
				else {
					cbuffer_send[0] = option;
					ntr = 1;
				}
			}

			gotoxy(33, 60);
			if (option == 1) {
				printf("你想提示哪個玩家？");
				gotoxy(34, 60);
				if (playerme[4] == 1) printf("  2  3  4");
				if (playerme[4] == 2) printf("  1  3  4");
				if (playerme[4] == 3) printf("  1  2  4");
				if (playerme[4] == 4) printf("  1  2  3");
				player = inoutkk(34, 60, 3, 3);
				if (playerme[4] == 1) player++;
				if (playerme[4] == 2) {
					player++;
					if (player == 2) player--;
				}
				if (playerme[4] == 3) {
					if (player == 3) player++;
				}
				cbuffer_send[1] = player;
				gotoxy(35, 60);
				printf("你想提示哪種線索？");
				gotoxy(36, 60);
				printf("  色彩  數字");
				CorN = inoutkk(36, 60, 2, 6);
				cbuffer_send[2] = CorN;
				gotoxy(37, 60);
				if (CorN == 1) {
					printf("選擇想提示的顔色");
					gotoxy(38, 60);
					printf("  紅色  綠色  黃色  藍色  白色");
					clueColor = inoutkk(38, 60, 5, 6);
					cbuffer_send[3] = clueColor;
				}
				if (CorN == 2) {
					printf("選擇想提示的數字");
					gotoxy(38, 60);
					printf("  1  2  3  4  5");
					clueNum = inoutkk(38, 60, 5, 3);
					cbuffer_send[3] = clueNum;
				}
				cbuffer_send[4] = 0;
				cbuffer_send[5] = 0;
				cbuffer_send[6] = playerme[4];
			}

			else if (option == 2) {
				cbuffer_send[1] = 0;
				cbuffer_send[2] = 0;
				cbuffer_send[3] = 0;

				printf("選一張牌丟掉");
				gotoxy(34, 60);
				printf("  1  2  3  4");
				abdcard = inoutkk(34, 60, 4, 3);
				cbuffer_send[4] = abdcard;
				cbuffer_send[5] = 0;
				cbuffer_send[6] = playerme[4];

				/* debug用
				printf("\nkk = ");
				for (kk = 0; kk < 7; kk++)
				printf("%d  ", cbuffer_send[kk]);
				printf("\n");
				*/


			}
			else if (option == 3) {
				cbuffer_send[1] = 0;
				cbuffer_send[2] = 0;
				cbuffer_send[3] = 0;
				cbuffer_send[4] = 0;
				cbuffer_send[6] = playerme[4];

				printf("選一張想出的牌");
				gotoxy(34, 60);
				printf("  1  2  3  4");
				drwcard = inoutkk(34, 60, 4, 3);
				cbuffer_send[5] = drwcard;
			}

			send(hServer, (char*)cbuffer_send, sizeof(cbuffer_send), 0);
		}
		for (i = 0; i < 8; i++) {
			gotoxy(31 + i, 60);
			printf("                                  ");
		}

		recv(hServer, (char*)cbuffer_recv, sizeof(cbuffer_recv), 0);

		gotoxy(31, 10);
		printf("                                            ");
		gotoxy(32, 10);
		printf("                                            ");
		gotoxy(31, 10);
		printf("玩家%d動作:", cbuffer_recv[60]);
		gotoxy(32, 10);
		if (cbuffer_recv[0] == 1) {
			printf("玩家%d有", cbuffer_recv[1]);

			switch (cbuffer_recv[2]) {
			case 1:
				if (cbuffer_recv[3] == 1) {
					printf(" %d張紅牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 2) {
					printf(" %d張綠牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 3) {
					printf(" %d張黃牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 4) {
					printf(" %d張藍牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else {
					printf(" %d張白牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				break;

			case 2:
				if (cbuffer_recv[3] == 1) {
					printf(" %d張號碼 1的牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 2) {
					printf(" %d張號碼 2的牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 3) {
					printf(" %d張號碼 3的牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else if (cbuffer_recv[3] == 4) {
					printf(" %d張號碼 4的牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				else {
					printf(" %d張號碼 5的牌 ", cbuffer_recv[4]);
					if (cbuffer_recv[4] != 0) {
						printf("在第 ");
						for (i = 0; i < cbuffer_recv[4]; i++)
							printf("%d ", cbuffer_recv[i + 5]);
						printf("位");
					}
				}
				break;
			}
		}

		else if (cbuffer_recv[9] == 2) {
			printf("玩家棄牌!");
			for (r = 0; r < 20; r++) {
				if (r >= 0 && r < 5) {
					player1[r] = cbuffer_recv[r + 15];
				}
				else if (r > 4 && r < 10) {
					player2[r % 5] = cbuffer_recv[r + 15];
				}
				else if (r > 9 && r < 15) {
					player3[r % 5] = cbuffer_recv[r + 15];
				}
				else if (r > 14 && r < 20) {
					playerme[r % 5] = cbuffer_recv[r + 15];
				}
			}

			//清空buffer
			/*
			printf("\n");
			for (n = 0; n < m; n++) {
			cbuffer_recv[n] = 0;
			}
			printf("\n");
			*/
			Game_Drawing_Half();

			gotoxy(22, 52);
			printf("%d", playerme[4]);

			gotoxy(18, 3);
			printf("%d", player1[4]);
			x = 6;
			y = 9;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player1[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				}
				switch (card_number[player1[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				}
				y = y + 5;
			}

			gotoxy(1, 52);
			printf("%d", player2[4]);
			y = 3;
			x = 61;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player2[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				default: break;
				}
				switch (card_number[player2[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				default: break;
				}
				x = x - 10;
			}

			gotoxy(18, 96);
			printf("%d", player3[4]);
			x = 86;
			y = 24;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player3[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				default: break;
				}
				switch (card_number[player3[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				default: break;
				}
				y = y - 5;

			}
		}

		else if (cbuffer_recv[10] == 3) {

			//成功出牌
			if (cbuffer_recv[11] == 1) {
				printf("出牌成功！！！");
				score++;

				//出成功的牌(們)更新
				for (s = 35; s < 60; s++) {
					if (s > 34 && s < 40)
						red[s % 5] = cbuffer_recv[s];

					else if (s > 39 && s < 45)
						green[s % 5] = cbuffer_recv[s];

					else if (s > 44 && s < 50)
						yellow[s % 5] = cbuffer_recv[s];

					else if (s > 49 && s < 55)
						blue[s % 5] = cbuffer_recv[s];

					else
						white[s % 5] = cbuffer_recv[s];
				}
			}


			//出牌失敗，成功的牌不更新
			else if (cbuffer_recv[11] == 2) {
				printf("出牌失敗！BONG～～～");
			}

			//玩家牌更新
			for (r = 0; r < 20; r++) {
				if (r >= 0 && r < 5) {
					player1[r] = cbuffer_recv[r + 15];
				}
				else if (r > 4 && r < 10) {
					player2[r % 5] = cbuffer_recv[r + 15];
				}
				else if (r > 9 && r < 15) {
					player3[r % 5] = cbuffer_recv[r + 15];
				}
				else if (r > 14 && r < 20) {
					playerme[r % 5] = cbuffer_recv[r + 15];
				}
			}

			Game_Drawing_Half();

			gotoxy(22, 52);
			printf("%d", playerme[4]);

			gotoxy(18, 3);
			printf("%d", player1[4]);
			x = 6;
			y = 9;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player1[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				}
				switch (card_number[player1[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				}
				y = y + 5;
			}

			gotoxy(1, 52);
			printf("%d", player2[4]);
			y = 3;
			x = 61;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player2[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				default: break;
				}
				switch (card_number[player2[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				default: break;
				}
				x = x - 10;
			}

			gotoxy(18, 96);
			printf("%d", player3[4]);
			x = 86;
			y = 24;
			for (i = 0; i<4; i++) {
				gotoxy(y, x);
				switch (card_color[player3[i]]) {
				case 'R': cout << "紅色"; break;
				case 'G': cout << "綠色"; break;
				case 'Y': cout << "黃色"; break;
				case 'B': cout << "藍色"; break;
				case 'W': cout << "白色"; break;
				default: break;
				}
				switch (card_number[player3[i]]) {
				case 1: cout << "1  "; break;
				case 2: cout << "2  "; break;
				case 3: cout << "3  "; break;
				case 4: cout << "4  "; break;
				case 5: cout << "5  "; break;
				default: break;
				}
				y = y - 5;

			}

		}
		bomb = cbuffer_recv[12];
		point = cbuffer_recv[13];
		remain = cbuffer_recv[14];
		pointer = cbuffer_recv[15];
		gotoxy(4, 14);
		printf("%d", point);
		gotoxy(5, 14);
		printf("%d", bomb);
		gotoxy(6, 14);
		printf("%d", remain);
		for (i = 0;; i++) {
			if (red[i] != 0) {
				set_console_color(0x40);
				gotoxy(11 + i * 2, 21);
				printf(" %d        ", i + 1);
				gotoxy(12 + i * 2, 21);
				printf("__________", i + 1);
			}
			else break;
		}
		for (i = 0;; i++) {
			if (green[i] != 0) {
				set_console_color(0x20);
				gotoxy(11 + i * 2, 33);
				printf(" %d        ", i + 1);
				gotoxy(12 + i * 2, 33);
				printf("__________", i + 1);
			}
			else break;
		}
		for (i = 0;; i++) {
			if (yellow[i] != 0) {
				set_console_color(0xe0);
				gotoxy(11 + i * 2, 45);
				printf(" %d        ", i + 1);
				gotoxy(12 + i * 2, 45);
				printf("__________", i + 1);
			}
			else break;
		}
		for (i = 0;; i++) {
			if (blue[i] != 0) {
				set_console_color(0x90);
				gotoxy(11 + i * 2, 57);
				printf(" %d        ", i + 1);
				gotoxy(12 + i * 2, 57);
				printf("__________", i + 1);
			}
			else break;
		}
		for (i = 0;; i++) {
			if (white[i] != 0) {
				set_console_color(0xf0);
				gotoxy(11 + i * 2, 69);
				printf(" %d        ", i + 1);
				gotoxy(12 + i * 2, 69);
				printf("__________", i + 1);
			}
			else break;
		}
		set_console_color(0x0f);
	}
	system("cls");
	End_Drawing();

	gotoxy(13, 51);
	printf("%d", score);
	gotoxy(30, 50);

#ifdef __WINDOWS
	closesocket(hServer);

#endif

#ifdef __MAC
	close(hServer);
#endif

#ifdef __WINDOWS
	system("pause");
	return EXIT_SUCCESS;
#endif

	return 0;
}

void Clr()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '),
		dwConSize,
		coordScreen,
		&cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole,
		csbi.wAttributes,
		dwConSize,
		coordScreen,
		&cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);

}

void gotoxy(int y, int  x)
{
	COORD point;
	point.X = x, point.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void Start_Screen(int row, int column)
{
	char start_screen[40][103] =
	{
		{ "                                                                                                  \n" },//1
	{ "                               ╲   *   ╱               ------------                               \n" },//2
	{ "                                  * * *                ------▊---------           ---------        \n" },//3
	{ "                 M              * * * * *          ----------█----------         ---█------       \n" },//4
	{ "          ;      c      N         * * *          ---◢█---▊-----------          ----█------      \n" },//5
	{ "            ;     c    ;       ╱   *   ╲       ----▊---▌----------             ---█--------    \n" },//6
	{ "            Bn  Bm   B              |            -------◥--▅--------             ---▊--▌------  \n" },//7
	{ "      Nm.     c nn...     mn;       .            ----◢██▌-------              ----▌--█------- \n" },//8
	{ "         mB nnn:   nn: Bm           .          --◢███◤--------           --------▌-██------ \n" },//9
	{ "           cn:;ccccc nn             .          ---█◤-█-----▆---           ----▎◢▌█◤------  \n" },//10
	{ "  mnN B Bmnnn cccccc:nnnnB:BcN N                 ----█--█--█----           ---█▌▌--------     \n" },//11
	{ "           nn;;ccccc nm                         ----▊---▊█◤---             ---▼▊▌-----       \n" },//12
	{ "         Bm ;nn    nnc.Bm                       -◢█---◥█◤---                 --▊-▊----       \n" },//13
	{ "      Nc      ; nn ;.     mn                    -▌ ▊---█---                   --█----█--       \n" },//14
	{ "            B:  Bm   B                        -▎--█----▊-       -▎-          -█-------█--     \n" },//15
	{ "           :     c    c                           -█----▌-      -▊--        -█◤--   ----██-  \n" },//16
	{ "          .      :      c           ::            -▊-----◥███◤-          ---          --▼--  \n" },//17
	{ "                 B            nm    :::    nm      ----   ---------                           ---   \n" },//18
	{ "                 B            nnn; .:::  nnn.                                                       \n" },//19
	{ "                 0       :.    nnn; ::: nnnc    .                 *       *       *                 \n" },//20
	{ "                 |       :::::: n   .   :::::::::                ∫      ∫      ∫                 \n" },//21
	{ "                 |         :::           .:::::              ╔═══╦═══╦═══╗             \n" },//22
	{ "                 .     ;mnnnm               .                ║______║______║______║             \n" },//23
	{ "                 .    nnnnnnn              mnnnnnn           ║      ║      ║      ║             \n" },//24
	{ "                 .          .              mnnnnn            ║  遊  ║  遊  ║  遊  ║             \n" },//25
	{ "                 .       :::::            ::::               ║  戲  ║  戲  ║  戲  ║             \n" },//26
	{ "                 .     :::::  nnm      cn  :::::             ║  開  ║  規  ║  結  ║             \n" },//27
	{ "                             nnnn :::  nnn                   ║  始  ║  則  ║  束  ║             \n" },//28
	{ "                            mnn   :::   nnn                  ║      ║      ║      ║             \n" },//29
	{ "                                  :::                        ║      ║      ║      ║             \n" },//30
	{ "                                   .                         ║      ║      ║      ║             \n" },//31
	{ "                                   .                         ╚═══╩═══╩═══╝             \n" },//32  
	{ "                                   .                                                                \n" },//33 
	{ "                                   .                                                                \n" }//34  
	};
	printf("%c", start_screen[row][column]);
}

void Game_Screen(int row, int column)
{
	char game_screen[40][103] =
	{
	{ "                                                                                                     \n" },//1
	{ "                                               玩家                                                 \n" },//2
	{ "                             ╔════╦════╦════╦════╗                             \n" },//3
	{ "                             ║        ║        ║        ║        ║               一天一蘋果    \n" },//4
	{ "    提示點數：               ║        ║        ║        ║        ║               醫生遠離我    \n" },//5
	{ "    炸    彈：               ║    ╔═╣    ╔═╣    ╔═╣    ╔═╣               (O ▼ O)/     \n" },//6
	{ "    剩餘牌數：               ║    ║ 4║    ║ 3║    ║ 2║    ║ 1║               飯要喫飽喲    \n" },//7
	{ "                             ╚══╩═╩══╩═╩══╩═╩══╩═╝                             \n" },//8
	{ "    ╔════╗                                                                    ╔════╗    \n" },//9
	{ "    ║        ║         紅          綠          黃          藍          白         ║        ║    \n" },//10
	{ "    ║        ║                                                                    ║        ║    \n" },//11
	{ "    ║    ╔═╣                                                                    ║    ╔═╣    \n" },//12
	{ "    ║    ║ 1║                                                                    ║    ║ 4║    \n" },//13
	{ "    ╠══╩═╣                                                                    ╠══╩═╣    \n" },//14
	{ "    ║        ║                                                                    ║        ║    \n" },//15
	{ "    ║        ║                                                                    ║        ║    \n" },//16
	{ "  玩║    ╔═╣                                                                    ║    ╔═╣玩  \n" },//17
	{ "  家║    ║ 2║                                                                    ║    ║ 3║家  \n" },//18
	{ "    ╠══╩═╣                                                                    ╠══╩═╣    \n" },//19
	{ "    ║        ║                                                                    ║        ║    \n" },//20
	{ "    ║        ║                                                                    ║        ║    \n" },//21
	{ "    ║    ╔═╣                                                                    ║    ╔═╣    \n" },//22
	{ "    ║    ║ 3║                               玩家                                 ║    ║ 2║    \n" },//23
	{ "    ╠══╩═╣             ╔════╦════╦════╦════╗             ╠══╩═╣    \n" },//24
	{ "    ║        ║             ║███◤║███◤║███◤║███◤║             ║        ║    \n" },//25
	{ "    ║        ║             ║██◤  ║██◤  ║██◤  ║██◤  ║             ║        ║    \n" },//26
	{ "    ║    ╔═╣             ║█◤╔═╣█◤╔═╣█◤╔═╣█◤╔═╣             ║    ╔═╣    \n" },//27
	{ "    ║    ║ 4║             ║◤  ║ 1║◤  ║ 2║◤  ║ 3║◤  ║ 4║             ║    ║ 1║    \n" },//28
	{ "    ╚══╩═╝             ╚══╩═╩══╩═╩══╩═╩══╩═╝             ╚══╩═╝    \n" },//29
	{ "    ╔════════════════════════╗  ╔═════════════════╗    \n" },//30
	{ "    ║◤╱  ╱            動作記錄                    ║  ║◤╱  ╱     動作選擇             ║    \n" },//31
	{ "    ║                                                ║  ║                                  ║    \n" },//32  
	{ "    ║                                                ║  ║                                  ║    \n" },//33 
	{ "    ║                                                ║  ║                                  ║    \n" },//34
	{ "    ║                                                ║  ║                                  ║    \n" },//35
	{ "    ║                                                ║  ║                                  ║    \n" },//36
	{ "    ║                                                ║  ║                                  ║    \n" },//37
	{ "    ║                                                ║  ║                                  ║    \n" },//38
	{ "    ║                                                ║  ║                                  ║    \n" },//39
	{ "    ╚════════════════════════╝  ╚═════════════════╝    \n" }//40
	};
	printf("%c", game_screen[row][column]);
}

void Rule_Screen(int row, int column)
{
	char rule_screen[40][103] =
	{
		{ "                                                                                                    \n" },//0
	{ "╔════════════════════════════════════════════════╗\n" },//1
	{ "║ 遊戲規則:                                                                                      ║\n" },//2
	{ "║                                                                                                ║\n" },//3
	{ "║ >> 玩家1,2,3,4依序選擇要做的動作                                                               ║\n" },//4
	{ "║ >> 能選擇的動作一共有三種: 提示、棄牌、出牌                                                    ║\n" },//5
	{ "║                                                                                                ║\n" },//6
	{ "║  ┌────────────┐    ┌────────────┐    ┌────────────┐  ║\n" },//7
	{ "║  │提示:                   │    │棄牌:                   │    │出牌:                   │  ║\n" },//8
	{ "║  │*一次只能提示一個玩家   │    │*棄掉手中的某一張牌     │    │*出牌錯誤時，即會引爆一 │  ║\n" },//9
	{ "║  │*只能提示“某個顏色”   │    │*棄牌過程不能看牌       │    │ 個炸彈。               │  ║\n" },//10
	{ "║  │ 或“某個數字           │    │*棄牌後即可重新獲得提示 │    │*出牌成功時，牌面會自動 │  ║\n" },//11
	{ "║  │*提示一次會消耗一個點數 │    │ 點數一點               │    │ 排到場中央。           │  ║\n" },//12
	{ "║  │*提示點數=0，不可再提示 │    │                        │    │                        │  ║\n" },//13
	{ "║  └────────────┘    └────────────┘    └────────────┘  ║\n" },//14
	{ "║                                                                                                ║\n" },//15
	{ "║ >> 遊戲結束狀況:                                                                               ║\n" },//16
	{ "║    1. 引爆三顆炸彈                                                                             ║\n" },//17
	{ "║    2. 拼湊出所有煙花(五種顏色的五個數字都完成排列)                                             ║\n" },//18
	{ "║    3. 牌庫的牌抽到最後一張時，該玩家進行最後一次動作，遊戲結束。                               ║\n" },//19
	{ "║                                                                                                ║\n" },//20
	{ "║ >> 遊戲計分:                                                                                   ║\n" },//21
	{ "║    成功出牌的牌數為最終總分數!                                                                 ║\n" },//22
	{ "║                                                                                                ║\n" },//23
	{ "║                               這是一個合作遊戲，玩家之間沒有輸贏                               ║\n" },//24
	{ "║                                 大家要一起合作牌出完美的煙火喔~                                ║\n" },//25
	{ "║                                                                                                ║\n" },//26
	{ "║                                         ∫開始挑戰吧!                                          ║\n" },//27
	{ "╚════════════════════════════════════════════════╝\n" },//28
	{ "                                                                                                    \n" },//29
	{ "                                                                                                    \n" }//30
	};
	printf("%c", rule_screen[row][column]);
}

void Connect_Screen(int row, int column)
{
	char connect_screen[40][103] =
	{
		{ "                                                                                                    \n" },//1
	{ "            ╔══════════════════════╗                                        \n" },//2
	{ "              IP:                                                                                   \n" },//3
	{ "            ╚══════════════════════╝                                        \n" },//4
	{ "            ╔══════════════════════╗                                        \n" },//5
	{ "              Port:                                                                                 \n" },//6
	{ "            ╚══════════════════════╝                                        \n" },//7
	{ "                                                                                                    \n" },//8
	{ "                                                                                                    \n" },//9
	{ "                                                                                                    \n" },//10
	{ "                                                                                                    \n" },//11
	{ "                                                                                                    \n" },//12
	{ "                                                                                                    \n" },//13
	{ "                                                                                                    \n" },//14
	{ "                                                                                                    \n" },//15
	};
	printf("%c", connect_screen[row][column]);
}

void Start_Drawing()
{
	int i, j;

	set_console_color(0x0f);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i<40; i++)
	{
		for (j = 2; j<103; j++)
		{
			Start_Screen(i, j);
		}
	}
	//////////////////////////////////////////////////////////////////
	gotoxy(1, 0);//////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(1, j);
		if (j >= 32 && j <= 39) set_console_color(0x0d);
		else if (j >= 56 && j <= 67) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(2, 0);//////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(2, j);
		if (j >= 32 && j <= 39) set_console_color(0x0b);
		else if (j >= 54 && j <= 70) set_console_color(0x3c);
		else if (j >= 82 && j <= 90) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(3, 0);//////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(3, j);
		if (j >= 14 && j <= 18) set_console_color(0x0c);
		else if (j >= 30 && j <= 39) set_console_color(0x0d);
		else if (j >= 50 && j <= 71) set_console_color(0x3c);
		else if (j >= 81 && j <= 91) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(4, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(4, j);
		if (j >= 8 && j <= 24) set_console_color(0x0d);
		else if (j >= 33 && j <= 37) set_console_color(0x0b);
		else if (j >= 48 && j <= 70) set_console_color(0x3c);
		else if (j >= 81 && j <= 92) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(5, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(5, j);
		if (j >= 4 && j <= 28) set_console_color(0x0c);
		else if (j >= 33 && j <= 36) set_console_color(0x0d);
		else if (j >= 48 && j <= 68) set_console_color(0x3c);
		else if (j >= 82 && j <= 94) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(6, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(6, j);
		if (j >= 2 && j <= 30) set_console_color(0x0d);
		else if (j >= 33 && j <= 36) set_console_color(0x0b);
		else if (j >= 48 && j <= 68) set_console_color(0x3c);
		else if (j >= 82 && j <= 96) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(7, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(7, j);
		if (j >= 1 && j <= 31) set_console_color(0x0c);
		else if (j >= 33 && j <= 36) set_console_color(0x0d);
		else if (j >= 48 && j <= 66) set_console_color(0x3c);
		else if (j >= 81 && j <= 97) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(8, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(8, j);
		if (j >= 1 && j <= 31) set_console_color(0x0d);
		else if (j >= 33 && j <= 36) set_console_color(0x0b);
		else if (j >= 46 && j <= 65) set_console_color(0x3c);
		else if (j >= 77 && j <= 97) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(9, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(9, j);
		if (j >= 0 && j <= 32) set_console_color(0x0c);
		else if (j >= 34 && j <= 35) set_console_color(0x0b);
		else if (j >= 46 && j <= 65) set_console_color(0x3c);
		else if (j >= 77 && j <= 96) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(10, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(10, j);
		if (j >= 0 && j <= 32) set_console_color(0x0d);
		else if (j >= 48 && j <= 65) set_console_color(0x3c);
		else if (j >= 77 && j <= 93) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(11, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(11, j);
		if (j >= 0 && j <= 32) set_console_color(0x0c);
		else if (j >= 47 && j <= 64) set_console_color(0x3c);
		else if (j >= 78 && j <= 91) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(12, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(12, j);
		if (j >= 1 && j <= 31) set_console_color(0x0d);
		else if (j >= 47 && j <= 63) set_console_color(0x3c);
		else if (j >= 81 && j <= 91) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(13, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(13, j);
		if (j >= 1 && j <= 31) set_console_color(0x0c);
		else if (j >= 47 && j <= 60) set_console_color(0x3c);
		else if (j >= 80 && j <= 91) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(14, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(14, j);
		if (j >= 2 && j <= 30) set_console_color(0x0d);
		else if (j >= 45 && j <= 58) set_console_color(0x3c);
		else if (j >= 66 && j <= 69) set_console_color(0x3c);
		else if (j >= 80 && j <= 93) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(15, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(15, j);
		if (j >= 4 && j <= 28) set_console_color(0x0c);
		else if (j >= 49 && j <= 58) set_console_color(0x3c);
		else if (j >= 65 && j <= 69) set_console_color(0x3c);
		else if (j >= 78 && j <= 84) set_console_color(0x3c);
		else if (j >= 88 && j <= 96) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(16, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(16, j);
		if (j >= 8 && j <= 24) set_console_color(0x04);
		else if (j >= 31 && j <= 37) set_console_color(0x0a);
		else if (j >= 49 && j <= 67) set_console_color(0x3c);
		else if (j >= 78 && j <= 80) set_console_color(0x3c);
		else if (j >= 91 && j <= 96) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(17, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(17, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 25 && j <= 43)set_console_color(0x0e);
		else if (j >= 50 && j <= 53) set_console_color(0x3c);
		else if (j >= 57 && j <= 65) set_console_color(0x3c);
		else if (j >= 93 && j <= 95) set_console_color(0x3c);
		else set_console_color(0x0f);
	}
	gotoxy(18, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(18, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 23 && j <= 45)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(19, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(19, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 21 && j <= 47)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(20, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(20, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(21, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(21, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(22, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(22, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(23, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(23, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 19 && j <= 49)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(24, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(24, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(25, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(25, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(26, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(26, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 20 && j <= 48)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(27, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(27, j);
		if (j >= 21 && j <= 47)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(28, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(28, j);
		if (j >= 23 && j <= 45)set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(29, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(29, j);
		if (j >= 25 && j <= 43)set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(30, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(30, j);
		if (j >= 31 && j <= 37)set_console_color(0x06);
		else set_console_color(0x0f);
	}
	gotoxy(31, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(31, j);
		if (j >= 31 && j <= 37)set_console_color(0x06);
		else set_console_color(0x0f);
	}
	gotoxy(32, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(32, j);
		if (j >= 31 && j <= 37)set_console_color(0x06);
		else set_console_color(0x0f);
	}
	gotoxy(33, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		Start_Screen(33, j);
		if (j >= 31 && j <= 37)set_console_color(0x06);
		else set_console_color(0x0f);
	}
}

void Game_Drawing()
{
	int i, j;

	set_console_color(15);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i < 40; i++)
	{
		for (j = 2; j < 103; j++)
		{
			Game_Screen(i, j);
		}
	}
}

void Game_Drawing_Half()
{
	int i, j;

	set_console_color(15);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i < 30; i++)
	{
		for (j = 2; j < 103; j++)
		{
			Game_Screen(i, j);
		}
	}
}

void Rule_Drawing()
{
	int i, j;

	set_console_color(15);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 103; j++)
		{
			Rule_Screen(i, j);
		}
	}
}

void Connect_Drawing()
{
	int i, j;

	set_console_color(15);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i < 40; i++)
	{
		for (j = 2; j < 103; j++)
		{
			Connect_Screen(i, j);
		}
	}
}

void set_console_color(unsigned short color_index)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
}

void Rule() {
	char key;
	Rule_Drawing();
	gotoxy(27, 45);
	key = _getch();
	while (1)
	{
		key = _getch();	//取得鍵盤輸入  Enter:13 
		if ((int)key == 13) break;//Enter只有一位元 						   	 
		else break;
	}
}

int inoutkk(int sy, int sx, int n, int d) {
	char kk[2];
	int flag;
	int x, y, lx, ly, o, lo;
	o = 1;
	gotoxy(sy, sx);
	y = sy;
	x = sx;
	flag = 0;
	gotoxy(y, x);
	set_console_color(0x0e);
	printf(">>");

	while (1) {
		kk[0] = _getch();
		kk[1] = _getch();
		if (kk[0] == 13) {
			set_console_color(0x0f);
			return o;
		}
		else {
			switch (kk[1]) {
			case 80://down
				lx = x;
				x = x + d;
				lo = o;
				o++;
				break;
			case 77://right
				lx = x;
				x = x + d;
				lo = o;
				o++;
				break;
			case 75://left
				lx = x;
				x = x - d;
				lo = o;
				o--;
				break;
			case 72://up
				lx = x;
				x = x - d;
				lo = o;
				o--;
				break;
			default:
				break;
			}
			if (x < sx || x >(sx + d * n - 1)) {
				x = lx;
				o = lo;
			}
			else {
				gotoxy(y, lx);
				set_console_color(0x0f);
				printf("  ");
				gotoxy(y, x);
				set_console_color(0x0e);
				printf(">>", o);
			}
		}
	}
}

void End_Screen(int row, int column)
{
	char end_screen[40][103] =
	{
		{ "                                                                                                    \n" },//0
	{ "                               ╲   *   ╱   ╲   *   ╱   ╲   *   ╱                              \n" },//1
	{ "                                  * * *         * * *         * * *                                 \n" },//2
	{ "                 M              * * * * *     * * * * *     * * * * *              M                \n" },//3
	{ "          ;      c      N         * * *         * * *         * * *         ;      c      N         \n" },//4
	{ "            ;     c    ;       ╱   *   ╲   ╱   *   ╲   ╱   *   ╲        ;     c    ;          \n" },//5
	{ "            Bn  Bm   B              |             |             |               Bn  Bm   B          \n" },//6
	{ "      Nm.     c nn...     mn;       .             .             .         Nm.     c nn...     mn;   \n" },//7
	{ "         mB nnn:   nn: Bm           .             .             .            mB nnn:   nn: Bm       \n" },//8
	{ "           cn:;ccccc nn             .             .             .              cn:;ccccc nn         \n" },//9
	{ "  mnN B Bmnnn cccccc:nnnnB:BcN N                                     mnN B Bmnnn cccccc:nnnnB:BcN N \n" },//10
	{ "           nn;;ccccc nm               ╔═══════════╗              nn;;ccccc nm          \n" },//11
	{ "         Bm ;nn    nnc.Bm                    遊戲結束!!!                    Bm ;nn    nnc.Bm        \n" },//12
	{ "      Nc      ; nn ;.     mn                  得分:  分                  Nc      ; nn ;.     mn     \n" },//13
	{ "            B:  Bm   B                ╚═══════════╝               B:  Bm   B           \n" },//14
	{ "           :     c    c                                                       :     c    c          \n" },//15
	{ "          .      :      c    ╔════════════════════╗    .      :      c        \n" },//16
	{ "                 B           ║煙火計分                                ║           B               \n" },//17
	{ "                 B           ║ 5分以下:慘不忍睹，觀眾噓聲四起。       ║           B               \n" },//18
	{ "                 0           ║ 6~10分:普普通通，只有零星的掌聲。      ║           0               \n" },//19
	{ "                 |           ║ 11~15分:值得一提，但大改很快就被遺忘。 ║           |               \n" },//20
	{ "                 |           ║ 16~20分:出色，觀眾非常滿意。           ║           |               \n" },//21
	{ "                 .           ║ 21~24分:太精采了!!!BRAVO!!!            ║           .               \n" },//22
	{ "                 .           ║ 25分:傳說中的完美煙火~~~               ║           .               \n" },//23
	{ "                 .           ╚════════════════════╝           .               \n" },//24
	{ "                 .                                                                  .               \n" },//25
	{ "                 .                                                                  .               \n" },//26
	{ "                                                                                                    \n" },//27
	{ "                                                                                                    \n" },//28
	{ "                                                                                                    \n" },//29
	{ "                                                                                                    \n" },//30
	{ "                                                                                                    \n" },//31  
	{ "                                                                                                    \n" },//32 
	{ "                                                                                                    \n" }//33  
	};
	printf("%c", end_screen[row][column]);
}

void End_Drawing()
{
	int i, j;

	set_console_color(0x0f);	//預設顏色	
	gotoxy(0, 0);//移動游標到初始位置
	for (i = 0; i<40; i++)
	{
		for (j = 2; j<103; j++)
		{
			End_Screen(i, j);
		}
	}
	//////////////////////////////////////////////////////////////////
	gotoxy(1, 0);//////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(1, j);
		if (j >= 32 && j <= 39) set_console_color(0x0e);
		else if (j >= 47 && j <= 53) set_console_color(0x0b);
		else if (j >= 61 && j <= 67) set_console_color(0x0d);
		else set_console_color(0x0f);
	}
	gotoxy(2, 0);//////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(2, j);
		if (j >= 32 && j <= 39) set_console_color(0x06);
		else if (j >= 45 && j <= 55) set_console_color(0x03);
		else if (j >= 59 && j <= 70) set_console_color(0x05);
		else set_console_color(0x0f);
	}
	gotoxy(3, 0);//////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(3, j);
		if (j >= 14 && j <= 18) set_console_color(0x0c);
		else if (j >= 30 && j <= 39) set_console_color(0x0e);
		else if (j >= 45 && j <= 55) set_console_color(0x0b);
		else if (j >= 59 && j <= 70) set_console_color(0x0d);
		else if (j >= 82 && j <= 85) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(4, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(4, j);
		if (j >= 8 && j <= 24) set_console_color(0x0d);
		else if (j >= 33 && j <= 37) set_console_color(0x06);
		else if (j >= 45 && j <= 55) set_console_color(0x03);
		else if (j >= 59 && j <= 70) set_console_color(0x05);
		else if (j >= 75 && j <= 94) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(5, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(5, j);
		if (j >= 4 && j <= 28) set_console_color(0x0c);
		else if (j >= 33 && j <= 36) set_console_color(0x0e);
		else if (j >= 47 && j <= 53) set_console_color(0x0b);
		else if (j >= 61 && j <= 67) set_console_color(0x0d);
		else if (j >= 75 && j <= 94) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(6, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(6, j);
		if (j >= 2 && j <= 30) set_console_color(0x0d);
		else if (j >= 33 && j <= 36) set_console_color(0x06);
		else if (j >= 47 && j <= 53) set_console_color(0x03);
		else if (j >= 63 && j <= 66) set_console_color(0x05);
		else if (j >= 75 && j <= 94) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(7, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(7, j);
		if (j >= 1 && j <= 31) set_console_color(0x0c);
		else if (j >= 33 && j <= 36) set_console_color(0x06);
		else if (j >= 47 && j <= 53) set_console_color(0x03);
		else if (j >= 59 && j <= 70) set_console_color(0x05);
		else if (j >= 73 && j <= 98) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(8, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(8, j);
		if (j >= 1 && j <= 31) set_console_color(0x0d);
		else if (j >= 33 && j <= 36) set_console_color(0x06);
		else if (j >= 47 && j <= 53) set_console_color(0x03);
		else if (j >= 59 && j <= 70) set_console_color(0x05);
		else if (j >= 73 && j <= 98) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(9, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(9, j);
		if (j >= 0 && j <= 32) set_console_color(0x0c);
		else if (j >= 34 && j <= 35) set_console_color(0x06);
		else if (j >= 47 && j <= 53) set_console_color(0x03);
		else if (j >= 59 && j <= 70) set_console_color(0x05);
		else if (j >= 73 && j <= 98) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(10, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(10, j);
		if (j >= 0 && j <= 32) set_console_color(0x0d);
		else if (j >= 68 && j <= 100) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(11, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(11, j);
		if (j >= 0 && j <= 32) set_console_color(0x0c);
		else if (j >= 72 && j <= 96) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(12, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(12, j);
		if (j >= 1 && j <= 31) set_console_color(0x0d);
		else if (j >= 73 && j <= 98) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(13, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(13, j);
		if (j >= 1 && j <= 31) set_console_color(0x0c);
		else if (j >= 72 && j <= 98) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(14, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(14, j);
		if (j >= 2 && j <= 30) set_console_color(0x0d);
		else if (j >= 76 && j <= 93) set_console_color(0x0a);
		else set_console_color(0x0f);
	}
	gotoxy(15, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(15, j);
		if (j >= 4 && j <= 28) set_console_color(0x0c);
		else if (j >= 76 && j <= 93) set_console_color(0x0e);
		else set_console_color(0x0f);
	}
	gotoxy(16, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(16, j);
		if (j >= 8 && j <= 24) set_console_color(0x04);
		else if (j >= 76 && j <= 93) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(17, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(17, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(18, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(18, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(19, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(19, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(20, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(20, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(21, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(21, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(22, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(22, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(23, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(23, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(24, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(24, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(25, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(25, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}
	gotoxy(26, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(26, j);
		if (j >= 14 && j <= 18) set_console_color(0x04);
		else if (j >= 83 && j <= 86) set_console_color(0x02);
		else set_console_color(0x0f);
	}

	gotoxy(27, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(27, j);
		set_console_color(0x0f);
	}

	gotoxy(28, 0);/////////////////
	for (j = 0; j<103; j++)
	{
		End_Screen(28, j);
		set_console_color(0x0f);
	}

}
