#include"all.h"

void displayAll(allObject *allObj)	//���������ϷԪ��
{
	beginPaint();					//��ʼ��ͼ

	if (state == PROCESS) {			//����Ϸ�ڽ��н׶�
		clearDevice();				//����
		displayBackground();		//�����Χ�߽�
		displayPlane(allObj->pll);	//����ɻ�
		displayBullet(allObj->bul);	//����ڵ�
		displayBattery(allObj->bat);//�������
		displayBomb(allObj->bol);	//�������
		displayCrash(allObj->crl);	//�����ը
		displayAward(allObj->awl);	//�������
		displayTime(gameTime);		//�����Ϸʱ��
		displayRecord();			//�����Ϸ��¼
	}
	else if (state == START) {		//����Ϸ�ڿ�ʼ�׶�
		clearDevice();
		displayBackground();
		displayFrame();				//�������������
		displayTitle("��������");	//�����Ϸ����
		displaySelection();			//�����Ϸ�Ѷ�ѡ��
	}
	else if (state == END) {		//����Ϸ�ڽ����׶�
		clearDevice();
		displayBackground();
		displayPlane(allObj->pll);	
		displayBullet(allObj->bul);
		displayBattery(allObj->bat);
		displayBomb(allObj->bol);
		displayCrash(allObj->crl);

		displayTime(gameTime);		//�����Ϸʱ��
		displayRestart();			//������¿�ʼѡ��
	}

	endPaint();			//������ͼ
}

void displayBackground(void)
{
	setBrushColor(WHITE);
	setPenColor(BLACK);
	setPenWidth(3);
	rectangle(0, 0, WIDTH, HEIGHT);
}

void displayPlane(planeList *pList)
{
	Plane *pl = pList->head;
	while (pl) {
		planeImg(pl);
		pl = pl->next;
	}
}

void planeImg(Plane *pl)
{
	setBrushColor(BLACK);
	setPenColor(BLACK);
	setPenWidth(1);
	rectangle(pl->p.x - 15, pl->p.y - 10, pl->p.x + 15, pl->p.y + 10);
}

void displayBomb(bombList *bList)
{
	Bomb *bo = bList->head;
	while (bo) {
		bombImg(bo);
		bo = bo->next;
	}
}

void bombImg(Bomb *bo)
{
	setBrushColor(RED);
	setPenColor(BLACK);
	setPenWidth(3);
	ellipse(bo->p.x - 10, bo->p.y - 10, bo->p.x + 10, bo->p.y + 10);
}

void displayBattery(Battery *ba)
{
	batteryImg(ba);
}

void batteryImg(Battery *ba)
{
	setBrushColor(RGB(255 * (1 - ba->blood / 100.0), 255 * ba->blood / 100.0, 0));
	setPenColor(BLACK);
	setPenWidth(3);
	ellipse(ba->p.x - 30, ba->p.y - 30, ba->p.x + 30, ba->p.y + 30);
}

void displayCrash(crashList *cList)
{
	Crash *cr = cList->head;
	while (cr) {
		crashImg(cr);
		cr = cr->next;
	}
}

void crashImg(Crash *cr)
{
	setBrushColor(YELLOW);
	setPenColor(MAGENTA);
	setPenWidth(3);
	ellipse(cr->p.x - cr->radius, cr->p.y - cr->radius, cr->p.x + cr->radius, cr->p.y + cr->radius);
}

void displayBullet(bulletList *bList)
{
	Bullet *bu = bList->head;
	while(bu) {
		bulletImg(bu);
		bu = bu->next;
	}
}

void bulletImg(Bullet *bu)
{
	setBrushColor(BLUE);
	setPenColor(CYAN);
	setPenWidth(3);
	ellipse(bu->p.x - 10, bu->p.y - 10, bu->p.x + 10, bu->p.y + 10);
}

void displayAward(awardList *aList)
{
	Award *aw = aList->head;
	while (aw) {
		AwardImg(aw);
		aw = aw->next;
	}
}

void AwardImg(Award *aw)
{
	switch (aw->type) {
	case 0:
		setBrushColor(BLUE);
		setPenColor(CYAN);
		setPenWidth(3);
		ellipse(aw->p.x - 15, aw->p.y - 15, aw->p.x + 15, aw->p.y + 15);
		break;
	case 1:
		setBrushColor(RGB(155, 255, 55));
		setPenColor(RGB(10, 155, 100));
		setPenWidth(3);
		ellipse(aw->p.x - 15, aw->p.y - 15, aw->p.x + 15, aw->p.y + 15);
		break;
	case 2:
		setBrushColor(RGB(233, 233, 233));
		setPenColor(BLACK);
		setPenWidth(2);
		rectangle(aw->p.x - 100, aw->p.y - 10, aw->p.x + 100, aw->p.y + 10);
		break;
	case 3:
		setBrushColor(RGB(255, 255, 155));
		setPenColor(RGB(100, 105, 250));
		setPenWidth(3);
		ellipse(aw->p.x - 15, aw->p.y - 15, aw->p.x + 15, aw->p.y + 15);
	default:
		break;
	}
}

void displayFrame(void) {
	setBrushColor(RGB(233, 233, 233));
	setPenColor(BLACK);
	setPenWidth(3);
	rectangle(WIDTH / 2 - 100, HEIGHT / 2 - 102, WIDTH / 2 + 100, HEIGHT / 2 + 102);
	rectangle(WIDTH / 2 - 65, HEIGHT / 2 - 102, WIDTH / 2 + 65, HEIGHT / 2 - 68);
	rectangle(WIDTH / 2 - 34, HEIGHT / 2 - 59.5, WIDTH / 2 + 34, HEIGHT / 2 - 25.5);
	rectangle(WIDTH / 2 - 34, HEIGHT / 2 - 17, WIDTH / 2 + 34, HEIGHT / 2 + 17);
	rectangle(WIDTH / 2 - 34, HEIGHT / 2 + 25.5, WIDTH / 2 + 34, HEIGHT / 2 + 59.5);
	rectangle(WIDTH / 2 - 34, HEIGHT / 2 + 68, WIDTH / 2 + 34, HEIGHT / 2 + 102);
}

void displayTitle(char *str)
{
	setTextBkColor(RGB(233, 233, 233));
	setTextSize(30);
	paintText(WIDTH/2 - 62, HEIGHT/2 - 100, str);
}

void displaySelection(void)
{
	setTextBkColor(RGB(233,233,233));
	setTextSize(30);
	paintText(WIDTH/2 - 31, HEIGHT/2 - 57.5, "��");
	paintText(WIDTH/2 - 31, HEIGHT/2 - 15, "�е�");
	paintText(WIDTH/2 - 31, HEIGHT/2 + 27.5, "����");
	paintText(WIDTH/2 - 31, HEIGHT/2 + 70, "ĩ��");
}

void displayTime(double gameTime)
{
	char* sTime = DtoS(gameTime); 

	if (state == PROCESS) {
		setTextBkColor(WHITE);
		setTextColor(RGB(30, 30, 30));
		setTextSize(20);
		paintText(500, 40, "��ǰ�ɼ���");
		if (gameTime >= record[difficulty]) {
			setTextColor(RED);
		}
		paintText(600, 40, sTime);
	}
	else if (state == END) {
		char* sRecord = DtoS(record[difficulty]);

		if (gameTime < record[difficulty]) {
			setBrushColor(RGB(233, 233, 233));
			setPenColor(BLACK);
			setPenWidth(3);
			rectangle(275, 250, 525, 301);
			rectangle(275, 300, 525, 385);
			rectangle(275, 200, 525, 240);
			rectangle(275, 160, 525, 201);

			setTextBkColor(RGB(233,233,233));
			setTextColor(BLACK);
			setTextSize(46);
			paintText(300, 252, "���ճɼ�");
			setTextSize(78);
			paintText(278, 303, sTime);

			setTextSize(34);
			paintText(328, 162, "��ѳɼ�");
			paintText(350, 204, sRecord);

			free(sTime);
			free(sRecord);
		}
		else {
			setBrushColor(RGB(233, 233, 233));
			setPenColor(BLACK);
			setPenWidth(3);
			rectangle(275, 210, 525, 261);
			rectangle(275, 260, 525, 345);

			setTextBkColor(RGB(233, 233, 233));
			setTextColor(BLACK);
			setTextSize(46);
			paintText(310, 212, "�¼�¼��");
			setTextSize(78);
			paintText(278, 263, sTime);
		}
	}
}

char* DtoS(double gameTime)				//��double��ʱ��תΪ�ַ���
{
	char* sTime;
	sTime = (char*)malloc(sizeof(char) * 7);

	int i;
	int iTime = gameTime;
	double dTime = gameTime - iTime;

	i = 1;
	while (i >= 0) {
		sTime[i--] = iTime % 10 + '0';
		iTime /= 10;
	}
	sTime[2] = '.';
	i = 3;
	while (i <= 4) {
		sTime[i++] = (int)(dTime * 10) + '0';
		dTime = dTime * 10 - (int)(dTime * 10);
	}
	sTime[5] = 's';
	sTime[6] = '\0';

	return sTime;
}

void displayRecord(void)
{
	if (state == PROCESS) {
		char* sRecord = DtoS(record[difficulty]);
		setTextBkColor(WHITE);
		setTextColor(RGB(30, 30, 30));
		setTextSize(20);
		paintText(500, 20, "��ѳɼ���");
		paintText(600, 20, sRecord);
		free(sRecord);
	}
}

void displayRestart(void)
{
	setBrushColor(RGB(233, 233, 233));
	setPenColor(BLACK);
	setPenWidth(3);
	rectangle(320, 400, 480, 441);

	setTextBkColor(RGB(233, 233, 233));
	setTextColor(BLACK);
	setTextSize(37);
	paintText(323, 402, "���¿�ʼ");
}