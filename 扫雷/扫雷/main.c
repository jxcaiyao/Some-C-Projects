//by??????��????~???? 
//????????????????��??����???????????? (i, j) ����????????????????????????��??����???????????? (x, y) ����???? ????����??��??��???????????????? (m, n) 
//mine???? % ����??????????��????@ ����??????????��
//show???? * ����????????????????????����??????��????????����??????????????????????��????# ����??????��??��??????????????�� 

#include "acllib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h> 
#include <math.h>

#define ROW 16
#define COL 30
#define MINE 99
#define LEFT 0
#define TOP 40
#define RIGHT 0
#define BOTTOM 0
#define CELL 30

char mine[ROW][COL];	//??????????��???????????? 
char show[ROW][COL];	//???????????????????????????? 
int state;				//0??????????????��????????1??????��????????-1??????���??  
int count;				//??????????????????????��????��????�� 
int left;				//????????��????????????��??��??????�� 
int seconds;

int surmine;			//??????????????��??��???? 
int unknown;			//??????????????��??????��????????????��????�� 
int flag;				//??????????????��??????��??????�� 
int suspect;			//??????????????��??????��??????��??��??????????????��???? 

						//??��????��????????????????��????��????????????????��????????????��??????????��?????????? 
void start(void);									//??????��???????? 
void initial(void);									//??��???????????????????????? 
void initBoard(void);								//???????????????????????????? 
void setMine(int m, int n);									//??��??????????��
void display(char show[ROW][COL]);					//??????????????��???????? 
void mouseEvent(int x, int y, int bt, int event);	//??����????������????����
void timerEvent(int tid);
int inBoard(int i, int j);							//????????(i, j)????��????????????????????????????����?????? 1??????????????����?????? 0 
void mineSweep(int i, int j);						//??��??��????����??��????????(i, j)????????????��????��????????????????��????????????????��
void noMine(int i, int j);							//??????????????��??????��??????��???????????? 0??????????��??????????????????????��????��????��
void typeMode(int i, int j, int mode);				//????????����??��??????�졤????��
void type(int i, int j, int mode);					//mode????0����??????????????????��??��????????mode????1����??????????????????��????mode????2����????????????????mode????3����????����????????????????0????mode????4����????����????????????????1
void assist(int i, int j);							//??��??��??????��????????????????????��????��???????????????????? 
int susMine1(int i, int j);
int susMine2(int i, int j);
int susMine3(int i, int j);

int Setup()
{
	initWindow("??��??��", DEFAULT, DEFAULT, COL*CELL + 1 + LEFT + RIGHT, ROW*CELL + 1 + TOP + BOTTOM);

	registerMouseEvent(mouseEvent);
	registerTimerEvent(timerEvent);

	start();
	//	display(mine);

	return 0;
}

void start(void)
{
	initial();
	//	setMine();
	display(show);
}

void initial(void)
{
	state = 2;
	count = 0;
	left = MINE;
	seconds = 0;
	initBoard();
}

void initBoard(void)
{
	int m, n;
	for (m = 0; m < ROW; m++) {
		for (n = 0; n < COL; n++) {
			mine[m][n] = '%';
			show[m][n] = '*';
		}
	}
}

void setMine(int m, int n)
{
	int i, j;
	int count = MINE;
	srand((unsigned)time(NULL));
	while (count) {
		i = rand() % ROW;
		j = rand() % COL;
		if (mine[i][j] == '%' && !(i == m && j == n)) {
			mine[i][j] = '@';
			count--;
		}
	}
}

void display(char show[ROW][COL])
{
	int i, j;

	beginPaint();

	setPenWidth(3);
	setPenColor(BLACK);
	setBrushColor(RGB(233, 233, 233));
	rectangle(0, 0, COL*CELL + 1 + LEFT + RIGHT, TOP + 1);
	rectangle(CELL*COL / 2.0 - 2 * TOP + LEFT, 0, CELL*COL / 2.0 + 2 * TOP + LEFT + 1, TOP + 1);
	setTextBkColor(RGB(233, 233, 233));
	setTextSize(TOP - 3);
	paintText(CELL*COL / 2.0 - 2 * TOP + LEFT + 3, 2, "????????????????");

	setPenWidth(3);
	setPenColor(BLACK);
	setBrushColor(RGB(233, 233, 233));
	rectangle(LEFT, 0, LEFT + TOP + 1, TOP + 1);
	char leftmines[1];
	_itoa_s(left, leftmines, 10, 10);
	setTextBkColor(RGB(233, 233, 233));
	setTextSize(TOP - 8);
	paintText(LEFT + 3, 2, leftmines);

	setPenWidth(3);
	setBrushColor(RGB(233, 233, 233));
	setTextBkColor(RGB(233, 233, 233));
	setPenColor(BLACK);
	setTextSize(TOP - 8);
	rectangle(CELL*COL - TOP + LEFT, 0, CELL*COL + LEFT, TOP + 1);
	char sec[1];
	_itoa_s(seconds, sec, 10, 10);
	paintText(CELL*COL - TOP + LEFT + 3, 2, sec);

	char number[1];
	setTextSize(CELL - 1);
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			setTextBkColor(WHITE);
			switch (show[i][j]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				_itoa_s(show[i][j] - '0', number, 10, 10);
				paintText(CELL*j + LEFT, CELL*i + TOP, "  ");
				paintText(CELL*j + (int)CELL / 4.0 + LEFT, CELL*i + TOP, number);
				break;
			case '0':
				setTextBkColor(RGB(255, 255, 255));
				paintText(CELL*j + LEFT, CELL*i + TOP, "  ");
				break;
			case '@':
				setBrushColor(RED);
				rectangle(CELL*j + LEFT, CELL*i + TOP, CELL*(j + 1) + 1 + LEFT, CELL*(i + 1) + 1 + TOP);
				break;
			case '%':
				break;
			case '#':
				setBrushColor(GREEN);
				rectangle(CELL*j + LEFT, CELL*i + TOP, CELL*(j + 1) + 1 + LEFT, CELL*(i + 1) + 1 + TOP);
				break;
			case '*':
				setTextBkColor(RGB(192, 192, 192));
				paintText(CELL*j + LEFT, CELL*i + TOP, "  ");
				break;
			case '!':
				setTextBkColor(RGB(192, 192, 192));
				paintText(CELL*j + LEFT, CELL*i + TOP, "  ");
				paintText(CELL*j + (int)CELL / 4.0 + LEFT, CELL*i + TOP, "@");
				break;
			case '?':
				setTextBkColor(RGB(192, 192, 192));
				paintText(CELL*j + LEFT, CELL*i + TOP, "  ");
				paintText(CELL*j + (int)CELL / 4.0 + LEFT, CELL*i + TOP, "?");
				break;
			}
		}
	}

	setPenWidth(3);
	for (i = 0; i < ROW + 1; i++)
		line(LEFT, CELL*i + TOP, CELL*COL + LEFT, CELL*i + TOP);
	for (j = 0; j < COL + 1; j++)
		line(CELL*j + LEFT, TOP, CELL*j + LEFT, CELL*ROW + TOP);
	endPaint();
}

void mouseEvent(int x, int y, int bt, int event)
{
	int i, j;
	if (event != BUTTON_DOWN)
		return;
	if (bt == LEFT_BUTTON && (state == 0 || state == 2) && x - LEFT>0 && y - TOP>0) {
		j = (x - LEFT) / CELL;
		i = (y - TOP) / CELL;
		if (state == 2) {
			setMine(i, j);
			startTimer(0, 1000);
			state = 0;
		}
		if (show[i][j] == '*') {
			mineSweep(i, j);
			if (state == 0) {
				display(show);
			}
			beginPaint();
			setPenWidth(3);
			setBrushColor(RGB(233, 233, 233));
			setTextBkColor(RGB(233, 233, 233));
			setPenColor(BLACK);
			setTextSize(TOP - 3);
			if (state == 1) {
				rectangle(CELL*COL / 2.0 - 2 * TOP + LEFT, 0, CELL*COL / 2.0 + 2 * TOP + LEFT + 1, TOP + 1);
				paintText(CELL*COL / 2.0 - 2 * TOP + LEFT + 3, 2, "????????????????");
			}
			else if (state == -1) {
				rectangle(CELL*COL / 2.0 - 2 * TOP + LEFT, 0, CELL*COL / 2.0 + 2 * TOP + LEFT + 1, TOP + 1);
				paintText(CELL*COL / 2.0 - 2 * TOP + LEFT + 3, 2, "????????????????");
			}
			endPaint();
		}
	}
	else if (bt == RIGHT_BUTTON && state == 0 && x - LEFT>0 && y - TOP>0) {
		j = (x - LEFT) / CELL;
		i = (y - TOP) / CELL;
		if (show[i][j] == '*') {
			show[i][j] = '!';
			left--;
		}
		else if (show[i][j] == '!') {
			show[i][j] = '?';
			left++;
		}
		else if (show[i][j] == '?') {
			show[i][j] = '*';
		}
		display(show);
	}
	else if (bt == LEFT_BUTTON && x>CELL*COL / 2.0 - 2 * TOP + LEFT - 1 && x<CELL*COL / 2.0 + 2 * TOP + LEFT + 1 && y>2 && y<TOP - 1) {	//???????????????? 
		start();
	}
}

void timerEvent(int tid)
{
	if (state == 2) {
		seconds = 0;
	}
	if (seconds < 99 && state == 0) {
		seconds++;
		beginPaint();
		setPenWidth(3);
		setBrushColor(RGB(233, 233, 233));
		setTextBkColor(RGB(233, 233, 233));
		setPenColor(BLACK);
		setTextSize(TOP - 8);
		rectangle(CELL*COL - TOP + LEFT, 0, CELL*COL + LEFT, TOP + 1);
		char sec[1];
		_itoa_s(seconds, sec, 10, 10);
		paintText(CELL*COL - TOP + LEFT + 3, 2, sec);
		endPaint();
	}
}

int inBoard(int i, int j)
{
	int ret = 0;
	if (i >= 0 && i<ROW && j >= 0 && j<COL)
		ret = 1;
	return ret;
}

void mineSweep(int i, int j)
{
	if (show[i][j] == '*' && state == 0 && inBoard(i, j)) {
		if (mine[i][j] == '@') {
			display(mine);
			startTimer(0, 1000);
			state = -1;
		}
		else {
			noMine(i, j);
			int m, n;
			for (m = i - 2; m <= i + 2; m++) {
				for (n = j - 2; n <= j + 2; n++) {
					//					if(m==i && n==j){
					//						continue;
					//					}
					assist(m, n);
				}
			}
		}
	}
}

void noMine(int i, int j)
{
	if (show[i][j] == '*' && state == 0 && inBoard(i, j)) {
		typeMode(i, j, 0);
		show[i][j] = surmine + '0';
		count++;
		if (count == ROW * COL - MINE) {
			state = 1;
			display(show);
		}
		if (surmine == 0) {
			typeMode(i, j, 1);
		}
	}
}

void typeMode(int i, int j, int mode)
{
	if (state == 0 && inBoard(i, j)) {
		if (mode == 0)
			surmine = 0;
		if (mode == 2)
			unknown = flag = suspect = 0;
		type(i - 1, j - 1, mode);
		type(i, j - 1, mode);
		type(i + 1, j - 1, mode);
		type(i - 1, j, mode);
		type(i + 1, j, mode);
		type(i - 1, j + 1, mode);
		type(i, j + 1, mode);
		type(i + 1, j + 1, mode);
	}
}

void type(int i, int j, int mode)	//mode????0����??????????????????��??��????????mode????1����??????????????????��????mode????2����????????????????mode????3����??????????��????mode????4����????����??????????��??????????�� ????mode????5����??????????????????��??????????�� 
{
	if (state == 0 && inBoard(i, j)) {
		if (mode == 0) {
			if (mine[i][j] == '@') {
				surmine++;
			}
		}
		else if (mode == 1) {
			if (show[i][j] == '*') {
				noMine(i, j);
			}
		}
		else if (mode == 2) {
			if (show[i][j] == '*') {
				unknown++;
			}
			else if (show[i][j] == '!') {
				flag++;
			}
			else if (show[i][j] == '#') {
				suspect++;
			}
		}
		else if (mode == 3) {
			if (show[i][j] == '*') {
				show[i][j] = '!';
				left--;
			}
		}
		else if (mode == 4) {
			if (show[i][j] == '*') {
				show[i][j] = '#';
			}
		}
		else if (mode == 5) {
			if (show[i][j] == '#') {
				show[i][j] = '*';
			}
		}
	}
}

void assist(int i, int j)
{
	if (state == 0 && inBoard(i, j) && show[i][j] >= '0' && show[i][j] <= '8') {
		int ret = 0;
		typeMode(i, j, 2);
		int leftmine = show[i][j] - '0' - flag;
		if (suspect == 2) {
			leftmine--;
		}
		if (unknown) {
			if (leftmine) {
				if (unknown == leftmine) {
					typeMode(i, j, 3);
					ret = 1;
				}
				else if (unknown == 2 && leftmine == 1) {
					ret = susMine1(i, j);					//???????? susMine1��??????????����????????????����??��????���� 
				}
				else if (unknown == 3 && leftmine == 2) {
					ret = susMine2(i, j);
				}
				else if (unknown == 3 && leftmine == 1) {
					ret = susMine3(i, j);
				}
			}
			else {
				typeMode(i, j, 1);
				ret = 1;
			}
		}
		if (ret == 1) {
			int m, n;
			for (m = 0; m < ROW; m++) {
				for (n = 0; n < COL; n++) {
					assist(m, n);
				}
			}
		}
	}
}

int susMine1(int i, int j)
{
	int ret = 0;
	if (state == 0 && inBoard(i, j) && show[i][j] >= '0' && show[i][j] <= '8') {
		typeMode(i, j, 4);
		int m, n;
		for (m = i - 2; m <= i + 2; m++) {
			for (n = j - 2; n <= j + 2; n++) {
				if (m == i && n == j) {
					continue;
				}
				if (show[m][n] >= '1' && show[m][n] <= '8' && inBoard(m, n)) {
					typeMode(m, n, 2);
					int leftmine = show[m][n] - '0' - flag;
					if (suspect == 2 && unknown >= 1 && leftmine >= 1) {
						leftmine--;
						if (leftmine == 0) {
							typeMode(m, n, 1);
							ret = 1;
						}
						else if (unknown == leftmine) {
							typeMode(m, n, 3);
							ret = 1;
						}
					}
				}
			}
		}
		typeMode(i, j, 5);
	}
	return ret;
}

int susMine2(int i, int j)
{
	int ret = 0;
	if (state == 0 && inBoard(i, j) && show[i][j] >= '0' && show[i][j] <= '8') {
		typeMode(i, j, 4);
		int m, n;
		for (m = i - 2; m <= i + 2; m++) {
			for (n = j - 2; n <= j + 2; n++) {
				if (m == i && n == j) {
					continue;
				}
				if (show[m][n] >= '1' && show[m][n] <= '8' && inBoard(m, n)) {
					typeMode(m, n, 2);
					int leftmine = show[m][n] - '0' - flag;
					if (suspect == 3 && unknown >= 1 && leftmine >= 2) {
						leftmine -= 2;
						if (leftmine == 0) {
							typeMode(m, n, 1);
							ret = 1;
						}
						else if (unknown == leftmine) {
							typeMode(m, n, 3);
							ret = 1;
						}
					}
					else if (suspect == 2 && leftmine == 1) {
						typeMode(m, n, 1);
					}
				}
			}
		}
		typeMode(i, j, 5);
	}
	return ret;
}

int susMine3(int i, int j)
{
	int ret = 0;
	if (state == 0 && inBoard(i, j) && show[i][j] >= '0' && show[i][j] <= '8') {
		typeMode(i, j, 4);
		int m, n;
		for (m = i - 2; m <= i + 2; m++) {
			for (n = j - 2; n <= j + 2; n++) {
				if (m == i && n == j) {
					continue;
				}
				if (show[m][n] >= '1' && show[m][n] <= '8' && inBoard(m, n)) {
					typeMode(m, n, 2);
					int leftmine = show[m][n] - '0' - flag;
					if (suspect == 3 && unknown >= 1 && leftmine >= 1) {
						leftmine--;
						if (leftmine == 0) {
							typeMode(m, n, 1);
							ret = 1;
						}
						else if (unknown == leftmine) {
							typeMode(m, n, 3);
							ret = 1;
						}
					}
				}
			}
		}
		typeMode(i, j, 5);
	}
	return ret;
}