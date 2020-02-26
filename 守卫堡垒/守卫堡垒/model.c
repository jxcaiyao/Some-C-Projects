#include"all.h"

const LIMIT PLANEX = { 0, 1 };				//0��ʾ�ɻ����������1��ʾ�ɻ����ҳ���
const LIMIT PLANEY = { HEIGHT / 10, 4 * HEIGHT / 10 };	//�ɻ���60��240�ĸ߶����ˢ��
const LIMIT PLANEVX = { WIDTH / 12, WIDTH / 8 };		//�ɻ�ˮƽ�����ٶȷ�ΧΪ67��100
const LIMIT PLANEVY = { 0, 0 };							//�ɻ���ֱ�����ٶ�Ϊ0

const int bombv[4] = { 200, 200, 200, 400 };			//�����Ѷ��µ����Ĳ�ͬ�����ٶ�
int BOMBV;

const int bombd[4] = { 5, 10, 20, 30 };					//�����Ѷ��µ���������
int BOMBD;

const int bulletv[4] = { 400, 400, 600, 800 };			//�����Ѷ����ڵ����ٶ�
int BULLETV;

const int CRASHT = 500;									//��ը����ʱ��Ϊ0.5s

const int planeperiod[4] = { 1000, 1000, 500, 250 };	//�����Ѷ��·ɻ�ˢ�µ����ڣ�����λ��ms��
int PLANEPERIOD;

const double bombprob[4] = { 0.5,0.6,0.7,0.8 };			//�����Ѷ��µ������ֵĸ���
double BOMBPROB;

const int bombperiod[4] = { 1000, 1000, 750, 500 };		//�����Ѷ��� �жϵ����Ƿ�ˢ�� ������
int BOMBPERIOD;

const int awardperiod[AWARDNUM][4] = {	{ 3000, 2000, 1000, 500 },
										{ 10000, 8000, 6000, 3000 },
										{ 5000, 4000, 3000, 2000 },
										{ 5000, 4000, 3000, 2000 } };	//�����Ѷ��� �жϲ����Ƿ�ˢ�� ������
int AWARDPERIOD[AWARDNUM];

const int AWARDT[AWARDNUM] = { 5000, 2000, 3000, 2000 };
const double AWARDPROB[AWARDNUM] = { 0.8, 0.7, 0.5, 0.5 };

const LIMIT AWARDPX[AWARDNUM] = { { 100, 700 }, { 200, 600 }, { 200, 600 }, { 100, 700 } };
const LIMIT AWARDPY[AWARDNUM] = { { 200, 550 }, { 300, 450 }, { 300, 400 }, { 200, 550 } };

allObject allObj;						//����������ϷԪ��
int state;								//��Ϸ״̬
double gameTime;						//һ����Ϸ���е�ʱ��
enum DIFFICULTYOFGAME difficulty;		//��Ϸ�Ѷ�

double record[4] = { 0 };				//��¼�����Ѷȵ���óɼ�

int random(int m, int n)				//��������������ޣ������һ������
{
	int randnum;
	static int i;
	srand(++i);							//ʹ����������Ϊ���ӿ��Ա����ڼ���ʱ�������������ͬ
	srand(rand() * (unsigned)time(NULL));
	//for (i = 0; i < 5; i++) {
	//	//srand((unsigned)time(NULL));
	//	srand(rand() * (unsigned)time(NULL));
	//}
	randnum = rand() % (n - m + 1) + m;
	if (i == 1000) {
		i = 0;
	}
	return randnum;
}

int sgn(double x)						//ȡ���ź���
{
	int ret = 1;
	if (fabs(x) < pow(10, -8)) {
		ret = 0;
	}
	else if (x < 0) {
		ret = -1;
	}
	return ret;
}

void addPlane(planeList *pList)			//����һ�ܷɻ�
{
	Plane *pl;
	pl = (Plane*)malloc(sizeof(Plane));
	pl->p.x = random((int)PLANEX.FLOOR, (int)PLANEX.CEIL) * WIDTH;
	pl->p.y = random((int)PLANEY.FLOOR, (int)PLANEY.CEIL);
	pl->v.vx = random((int)PLANEVX.FLOOR, (int)PLANEVX.CEIL);
	if (pl->p.x > 0) {
		pl->v.vx = -pl->v.vx;			//�ɻ����ұ߳���ʱע��ˮƽ�ٶ��Ǹ���
	}
	pl->v.vy = random((int)PLANEVY.FLOOR, (int)PLANEVY.CEIL);
	pl->next = NULL;
	if (pList->head) {
		pl->last = pList->tail;
		pList->tail = pList->tail->next = pl;
	}
	else {
		pList->head = pList->tail = pl;
		pList->head->last = NULL;
	}
}

void changePlane(planeList *pList)		//�ı�ɻ�λ��
{
	Plane *pl = pList->head;
	while (pl) {
		pl->p.x += pl->v.vx * INTERVAL / 1000;
		pl->p.y += pl->v.vy * INTERVAL / 1000;
		if (pl->p.x > WIDTH || pl->p.x < 0) {
			deletePlane(pList, &pl);	//����ɳ�������ɾ���˼ܷɻ�
		}
		else {
			pl = pl->next;
		}
	}
}

void deletePlane(planeList *pList, Plane **pl)	//ɾ�������е�ĳ�ܷɻ�
{
	if ((*pl)->last) {							//������������е�ͷһ��
		if ((*pl)->next) {						//������������е����һ��
			(*pl)->last->next = (*pl)->next;
			(*pl)->next->last = (*pl)->last;
		}
		else {									//����������е����һ��
			(*pl)->last->next = (*pl)->next;
			pList->tail = (*pl)->last;
		}
	}
	else if((*pl)->next){						//����������е�ͷһ�����Ҳ������һ��
		(*pl)->next->last = (*pl)->last;
		pList->head = (*pl)->next;
	}
	else {										//���������ͷһ����Ҳ����������һ��
		pList->head = pList->tail = NULL;
	}
	Plane *temp;
	temp = (*pl)->next;
	free(*pl);
	*pl = temp;
}

void freePlane(planeList *pList)				//ɾ�������ɻ�����
{
	Plane *p, *q;
	for (p = pList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addBomb(bombList *bList, Point p)			//����һ������
{
	Bomb *bo;
	bo = (Bomb*)malloc(sizeof(Bomb));
	bo->p.x = p.x;
	bo->p.y = p.y;
	
	double a;
	a = atan((allObj.bat->p.x - (double)p.x) / (allObj.bat->p.y - (double)p.y));
	bo->v.vx = /*(int)*/sgn(allObj.bat->p.y - (double)p.y) * (BOMBV * sin(a));		//����ָ����
	bo->v.vy = /*(int)*/sgn(allObj.bat->p.y - (double)p.y) * (BOMBV * cos(a));		
	//ע����Ϊatan��ȡֵ��Χ����������Ҫȡ���ź����ĸ���
	
	bo->next = NULL;
	if (bList->head) {
		bo->last = bList->tail;
		bList->tail = bList->tail->next = bo;
	}
	else {
		bList->head = bList->tail = bo;
		bList->head->last = NULL;
	}
}

void changeBomb(bombList *bList)				//�ı����е���λ��
{
	Bomb *bo = bList->head;
	while (bo) {
		bo->p.x += bo->v.vx * INTERVAL / 1000 * (0.1 * gameTime + 1);	//�����ٶ�����Ϸʱ����������
		bo->p.y += bo->v.vy * INTERVAL / 1000 * (0.1 * gameTime + 1);
		//double a;			//�˶�ע�Ϳ�ʹ����׷�ٱ���
		//a = atan((allObj.bat->p.x - bo->p.x) / (allObj.bat->p.y - bo->p.y));
		//bo->v.vx = /*(int)*/sgn(allObj.bat->p.y - bo->p.y) * (BOMBV * sin(a));
		//bo->v.vy = /*(int)*/sgn(allObj.bat->p.y - bo->p.y) * (BOMBV * cos(a));
		if (bo->p.x > WIDTH || bo->p.x < 0 || bo->p.y > HEIGHT || bo->p.y < 0) {
			deleteBomb(bList, &bo);
		}
		else {
			bo = bo->next;
		}
	}
}

void deleteBomb(bombList *bList, Bomb **bo)			//ɾ��ĳ������
{
	if ((*bo)->last) {
		if ((*bo)->next) {
			(*bo)->last->next = (*bo)->next;
			(*bo)->next->last = (*bo)->last;
		}
		else {
			(*bo)->last->next = (*bo)->next;
			bList->tail = (*bo)->last;
		}
	}
	else if((*bo)->next){
		(*bo)->next->last = (*bo)->last;
		bList->head = (*bo)->next;
	}
	else {
		bList->head = bList->tail = NULL;
	}
	Bomb *temp;
	temp = (*bo)->next;
	free(*bo);
	*bo = temp;
}

void freeBomb(bombList *bList)				//ɾ��������������
{
	Bomb *p, *q;
	for (p = bList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addBullet(bulletList *bList, double angle)		//����һ���ڵ�����angle�����
{
	Bullet *bu;
	bu = (Bullet*)malloc(sizeof(Bullet));
	bu->p.x = allObj.bat->p.x;
	bu->p.y = allObj.bat->p.y;
	bu->v.vx =  /*(int)*/(BULLETV * cos(angle));
	bu->v.vy = -/*(int)*/(BULLETV * sin(angle));
	bu->next = NULL;
	if (bList->head) {
		bu->last = bList->tail;
		bList->tail = bList->tail->next = bu;
	}
	else {
		bList->head = bList->tail = bu;
		bList->head->last = NULL;
	}
}

void changeBullet(bulletList *bList)				//�ı������ڵ�λ��
{
	Bullet *bu = bList->head;
	//Bomb *bo = allObj.bol->head;
	while (bu) {
		bu->p.x += bu->v.vx * INTERVAL / 1000;
		bu->p.y += bu->v.vy * INTERVAL / 1000;
		//double a;				//�˶�ע�Ϳ�ʹ�ڵ�׷�ٵ����������ã�
		//a = atan((bo->p.x - bu->p.x) / (bo->p.y - bu->p.y));
		//bu->v.vx = /*(int)*/sgn(bo->p.y - bu->p.y) * (BOMBV * sin(a));
		//bu->v.vy = /*(int)*/sgn(bo->p.y - bu->p.y) * (BOMBV * cos(a));
		if (bu->p.x > WIDTH || bu->p.x < 0 || bu->p.y > HEIGHT || bu->p.y < 0) {
			deleteBullet(bList, &bu);
		}
		else {
			bu = bu->next;
			//bo = bo->next;
			//if (bo == NULL) {
			//	bo = allObj.bol->head;
			//}
		}
	}
}

void deleteBullet(bulletList *bList, Bullet **bu)		//ɾ��һ���ڵ�
{
	if ((*bu)->last) {
		if ((*bu)->next) {
			(*bu)->last->next = (*bu)->next;
			(*bu)->next->last = (*bu)->last;
		}
		else {
			(*bu)->last->next = (*bu)->next;
			bList->tail = (*bu)->last;
		}
	}
	else if((*bu)->next){
		(*bu)->next->last = (*bu)->last;
		bList->head = (*bu)->next;
	}
	else {
		bList->head = bList->tail = NULL;
	}
	Bullet* temp = (*bu)->next;
	free(*bu);
	*bu = temp;
}

void freeBullet(bulletList *bList)			//ɾ�������ڵ�����
{
	Bullet *p, *q;
	for (p = bList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addCrash(crashList *cList, Point p, int radius)	//��pλ������һ���뾶Ϊradius�ı�ը
{
	Crash *cr;
	cr = (Crash*)malloc(sizeof(Crash));
	cr->p.x = p.x;
	cr->p.y = p.y;
	cr->time = CRASHT;				//��ըЧ������ʱ��
	cr->radius = radius;
	cr->next = NULL;
	if (cList->head) {
		cr->last = cList->tail;
		cList->tail = cList->tail->next = cr;
	}
	else {
		cList->head = cList->tail = cr;
		cList->head->last = NULL;
	}
}

void changeCrash(crashList *cList)			//�������б�ը����ʱ�䣬ʹ��ը��ʱ��ʧ
{
	Crash *cr = cList->head;
	while (cr) {
		cr->time -= INTERVAL;
		if (cr->time <= 0) {
			deleteCrash(cList, &cr);
		}
		else {
			cr = cr->next;
		}
	}
}

void deleteCrash(crashList *cList, Crash **cr)		//ɾ��ĳ����ը
{
	if ((*cr)->last) {
		if ((*cr)->next) {
			(*cr)->last->next = (*cr)->next;
			(*cr)->next->last = (*cr)->last;
		}
		else {
			(*cr)->last->next = (*cr)->next;
			cList->tail = (*cr)->last;
		}
	}
	else if((*cr)->next){
		(*cr)->next->last = (*cr)->last;
		cList->head = (*cr)->next;
	}
	else {
		cList->head = cList->tail = NULL;
	}
	Crash *temp = (*cr)->next;
	free(*cr);
	*cr = temp;
}

void freeCrash(crashList *cList)			//ɾ��������ը����
{
	Crash *p, *q;
	for (p = cList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addAward(awardList *aList, Point p, int type)	//����һ��������������
{
	Award *aw;
	aw = (Award*)malloc(sizeof(Award));
	aw->p.x = p.x;
	aw->p.y = p.y;
	aw->time = AWARDT[type];
	aw->type = type;
	aw->next = NULL;
	if (aList->head) {
		aw->last = aList->tail;
		aList->tail = aList->tail->next = aw;
	}
	else {
		aList->head = aList->tail = aw;
		aList->head->last = NULL;
	}
}

void changeAward(awardList *aList)		//���ٲ�������ʱ��
{
	Award *aw = aList->head;
	while (aw) {
		switch (aw->type) {
		case 0:
		case 1:
		case 2:
		case 3:
			aw->time -= INTERVAL;
			if (aw->time <= 0) {
				deleteAward(aList, &aw);
			}
			else {
				aw = aw->next;
			}
			break;
		}
	}
}

void deleteAward(awardList *aList, Award **aw)		//ɾ��ĳ������
{
	if ((*aw)->last) {
		if ((*aw)->next) {
			(*aw)->last->next = (*aw)->next;
			(*aw)->next->last = (*aw)->last;
		}
		else {
			(*aw)->last->next = (*aw)->next;
			aList->tail = (*aw)->last;
		}
	}
	else if ((*aw)->next) {
		(*aw)->next->last = (*aw)->last;
		aList->head = (*aw)->next;
	}
	else {
		aList->head = aList->tail = NULL;
	}
	Award *temp = (*aw)->next;
	free(*aw);
	*aw = temp;
}
void freeAward(awardList *aList)		//ɾ��������������
{
	Award *p, *q;
	for (p = aList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void changeBattery(Battery *ba)			//�ı䱤������
{
	if (ba->type0) {					//ʹ��0�Ͳ����������ͷ��ڵ�
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180);
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180 + 2 * PI / 3);
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180 + 4 * PI / 3);
		ba->type0 --;
	}
}

void reductBattery(Battery *ba, int damage)		//���ٱ���Ѫ��
{
	if (ba->blood - damage > 0) {
		ba->blood -= damage;
	}
	else {										//����Ѫ������
		ba->blood = 0;
		addCrash(allObj.crl, ba->p, 35);		//���ݱ�ը
		GameOver();								//��Ϸ����
	}
}

void initAll(allObject *allObj)					//��ʼ��������ϷԪ��
{
	planeList *pll;
	bombList *bol;
	bulletList *bul;
	crashList *crl;
	awardList *awl;
	Battery *bat;

	pll = (planeList*)malloc(sizeof(planeList));
	bol = (bombList*)malloc(sizeof(bombList));
	bul = (bulletList*)malloc(sizeof(bulletList));
	crl = (crashList*)malloc(sizeof(crashList));
	awl = (awardList*)malloc(sizeof(awardList));
	bat = (Battery*)malloc(sizeof(Battery));

	allObj->pll = pll;
	allObj->bol = bol;
	allObj->bul = bul;
	allObj->crl = crl;
	allObj->awl = awl;
	allObj->bat = bat;

	pll->head = pll->tail = NULL;
	bol->head = bol->tail = NULL;
	bul->head = bul->tail = NULL;
	crl->head = crl->tail = NULL;
	awl->head = awl->tail = NULL;
	
	bat->p.x = BATTERYX;		//���ݳ�ʼ��
	bat->p.y = BATTERYY;
	bat->blood = BATTERYB;
	bat->type0 = 0;				//0�Ͳ�����ʹ�ô���Ϊ0

	getRecord();				//���ĵ��еõ�������Ϸ��¼��߷�

	state = START;				//��Ϸ��ʼ

	gameTime = 0;				//��Ϸʱ���ʼ��Ϊ0
}

void setDifficulty(int df)		//����ѡ����Ϸ�Ѷ��趨����
{
	initAll(&allObj);			//��ʼ��������ϷԪ��

	//stopSound(0);
	//stopSound(2);
	//playSound(1, 1);

	BOMBV = bombv[df];				//�����ٶ�
	BOMBD = bombd[df];				//�����˺�
	BULLETV = bulletv[df];			//�ڵ��ٶ�
	BOMBPROB = bombprob[df];		//�������ɿ�����
	PLANEPERIOD = planeperiod[df];	//�ɻ���������
	BOMBPERIOD = bombperiod[df];	//���������ж�����

	int i;
	for (i = 0; i < AWARDNUM; i++) {
		AWARDPERIOD[i] = awardperiod[i][df];	//���������ж�����
	}

	difficulty = df;				//�洢��Ϸ�Ѷ�

	state = PROCESS;				//��Ϸ���ڽ��н׶�
}

void changeAll(allObject *allObj)		//�����иı���ϷԪ�صĺ������ϵ�һ��
{
	randPlane(allObj->pll);				//���ɷɻ�
	randBomb(allObj->bol, allObj->pll);	//������ɵ���
	randAward(allObj->awl);				//������ɲ���Ʒ

	changePlane(allObj->pll);
	changeBomb(allObj->bol);
	changeBullet(allObj->bul);
	changeCrash(allObj->crl);
	changeAward(allObj->awl);
	changeBattery(allObj->bat);
}

void GameOver(void)				//��Ϸ����
{
	state = END;				//��Ϸ��������׶�
	//stopSound(1);
	//playSound(2, 1);
	putRecord();				//����Ϸ�ɼ������ĵ�
}

int probability(double p)		//����1�ĸ���Ϊp,����0�ĸ���Ϊ1-p
{
	int ret = 0;
	if (random(0, 1000) <= 1000 * p) {
		ret = 1;
	}
	return ret;
}

void randPlane(planeList *pList)	//ÿ��һ��ʱ������һ�ܷɻ�
{
	static int i = 0;
	if (i == 0) {
			addPlane(pList);
			i = PLANEPERIOD / INTERVAL;
	}
	else {
		i--;
	}
}

void randBomb(bombList *bList, planeList *pList)	//ÿ��һ��ʱ���������һ�ε���
{
	Plane *pl = pList->head;
	static int i = 0;
	if (i <= 0) {
		while (pl) {
			if (probability(BOMBPROB * (0.005 * gameTime + 1))) {
				addBomb(bList, pl->p);
				i = BOMBPERIOD * exp(-gameTime * 0.05) / INTERVAL;	//����������������Ϸʱ��������
			}
			pl = pl->next;
		}
	}
	else {
		i--;
	}
}

void randAward(awardList *aList)	//ÿ��һ��ʱ���������һ�β���Ʒ
{
	static int at[5] = { 0 };
	int i;
	Point p;
	//static int count[5] = { 0 };
	//static int sum[5] = { 0 };

	//if (gameTime >= 1) {
	//	initConsole();
	//	printf("%d %d\n", sum[0], count[0]);
	//	printf("%d %d\n", sum[1], count[1]);
	//	GameOver();
	//}

	for (i = 0; i < AWARDNUM; i++) {
		if (at[i] <= 0) {
			//count[i]++;
			if (probability(AWARDPROB[i])) {
				//sum[i]++;
				p.x = random(AWARDPX[i].FLOOR, AWARDPX[i].CEIL);			//����Ʒ�������λ��
				p.y = random(AWARDPY[i].FLOOR, AWARDPY[i].CEIL);

				addAward(aList, p, i);
			}
			at[i] = AWARDPERIOD[i] / INTERVAL;
		}
		else {
			at[i]--;
		}
	}
}

void decideCrash(allObject *allObj)						//�ж���ײ�Ƿ���
{
	PlBuCrash(allObj->pll, allObj->bul, allObj->crl);	//�ɻ����ڵ���ײ
	BoBuCrash(allObj->bol, allObj->bul, allObj->crl);	//�������ڵ���ײ
	BoBaCrash(allObj->bol, allObj->bat, allObj->crl);	//�����뱤����ײ
	BoAwCrash(allObj->bol, allObj->awl, allObj->crl);	//�����벹����ײ
	AwBaCrash(allObj->awl, allObj->bat);				//�����뱤����ײ
}

void PlBuCrash(planeList *pList, bulletList *bList, crashList *cList)	//�ɻ����ڵ���ײ
{
	Bullet *bu = bList->head;
	while (bu) {
		Plane *pl = pList->head;
		while (pl) {
			if (PlBuCrashCondi(pl, bu)) {
				addCrash(cList, pl->p, 20);
				deletePlane(pList, &pl);
				deleteBullet(bList, &bu);
				break;
			}
			else {
				pl = pl->next;
			}
		}
		if (bu) {
			bu = bu->next;
		}
	}
}

int PlBuCrashCondi(Plane *pl, Bullet *bu)		//�ɻ����ڵ���ײ����1��δ��ײ����0
{
	int ret = 0;
	if (sqrt(pow(pl->p.x - bu->p.x, 2) + pow(pl->p.y - bu->p.y, 2)) <= 20) {	//���С��20
		ret = 1;
	}
	return ret;
}

void BoBuCrash(bombList *boList, bulletList *buList, crashList *crList)	//�������ڵ���ײ
{
	Bullet *bu = buList->head;
	while (bu) {
		Bomb *bo = boList->head;
		while (bo) {
			if (BoBuCrashCondi(bo, bu)) {
				addCrash(crList, bo->p, 20);
				deleteBomb(boList, &bo);
				deleteBullet(buList, &bu);
				break;
			}
			else {
				bo = bo->next;
			}
		}
		if (bu) {
			bu = bu->next;
		}
	}
}

int BoBuCrashCondi(Bomb *bo, Bullet *bu)		//�������ڵ���ײ����1��δ��ײ����0
{
	int ret = 0;
	if (sqrt(pow(bo->p.x - bu->p.x, 2) + pow(bo->p.y - bu->p.y, 2)) <= 20) {	//���С��20
		ret = 1;
	}
	return ret;
}

void BoBaCrash(bombList *boList, Battery *ba, crashList *crList)	//�����뱤����ײ
{
	Bomb *bo = boList->head;
	while (bo) {
		if (BoBaCrashCondi(bo, ba)) {
			addCrash(crList, bo->p, 20);
			deleteBomb(boList, &bo);
			reductBattery(ba, BOMBD);
		}
		else {
			bo = bo->next;
		}
	}
}

int BoBaCrashCondi(Bomb *bo, Battery *ba)		//�����뱤����ײ����1��δ��ײ����0
{
	int ret = 0;
	if (sqrt(pow(bo->p.x - ba->p.x, 2) + pow(bo->p.y - ba->p.y, 2)) <= 40) {	//���С��40
		ret = 1;
	}
	return ret;
}

void BoAwCrash(bombList *boList, awardList *awList, crashList *crList)
{
	Award *aw = awList->head;
	Bomb *bo = boList->head;

	while (aw) {
		switch (aw->type) {
		case 2:
			while (bo) {
				if (BoAwCrashCondi(bo, aw)) {
					addCrash(crList, bo->p, 20);
					deleteBomb(boList, &bo);
				}
				else {
					bo = bo->next;
				}
			}
			break;
		default:
			break;
		}
		aw = aw->next;
	}
}

int BoAwCrashCondi(Bomb *bo, Award *aw)
{
	int ret = 0; 

	switch (aw->type) {
	case 2:
		if (	bo->p.x - aw->p.x <= 120 && aw->p.x - bo->p.x <= 85
			&&	bo->p.y - aw->p.y <= 120 && aw->p.y - bo->p.y <= 28) {
			ret = 1;
		}
		break;
	default:
		break;
	}

	return ret;
}

void AwBaCrash(awardList *awList, Battery *ba)	//����Ʒ�뱤����ײ
{
	Award *aw = awList->head;
	Plane *pl = allObj.pll->head;
	Bomb *bo = allObj.bol->head;

	while (aw) {
		if (AwBaCrashCondi(aw, ba)) {
			switch (aw->type) {
			case 0:
				ba->type0 += 500 / INTERVAL;		//���ݿɻ��0.5��ӳ�
				deleteAward(awList, &aw);
				break;
			case 1:
				while (pl) {
					addCrash(allObj.crl, pl->p, 20);
					deletePlane(allObj.pll, &pl);
				}
				while (bo) {
					addCrash(allObj.crl, bo->p, 20);
					deleteBomb(allObj.bol, &bo);
				}
				deleteAward(awList, &aw);
				break;
			case 3:
				ba->blood = BATTERYB;
				deleteAward(awList, &aw);
				break;
			default:
				break;
			}
		}
		else {
			aw = aw->next;
		}
	}
}

int AwBaCrashCondi(Award *aw, Battery *ba)		//����Ʒ�뱤����ײ����1��δ��ײ����0
{
	int ret = 0;

	switch (aw->type) {
	case 0:
	case 1:
	case 3:
		if (sqrt(pow(aw->p.x - ba->p.x, 2) + pow(aw->p.y - ba->p.y, 2)) <= 45) {	//���С��45
			ret = 1;
		}
		break;
	default:
		break;
	}

	return ret;
}

void getRecord(void)				//�õ��ļ��е���Ϸ��¼
{
	FILE *fp;

	if (!fopen_s(&fp, "record.txt", "r")) {	//���ļ��ɹ�
		int i;

		for (i = 0; i < 4; i++) {
			fscanf_s(fp, "%lf", &record[i]);
		}

		if (fclose(fp)) {
			initConsole();
			printf("�޷��ر�record.txt�ļ���\n");
		}
	}
	else {					//���ļ�ʧ��
		//initConsole();
		//printf("�Ҳ���record.txt�ļ���\n");
		//state = END;
		fp = fopen("record.txt", "w+");		//�½����ļ�
		fclose(fp);
		getRecord();						//���¶�ȡ�ļ��еļ�¼
	}
}

void putRecord(void)			//������Ϸ��¼д���ĵ�
{
	FILE *fp;

	if (!fopen_s(&fp, "record.txt", "w")) {
		int i;

		if (gameTime > record[difficulty]) {	//�����������Ϸ��¼�����Ϊ�µļ�¼
			record[difficulty] = gameTime;
		}

		for (i = 0; i < 4; i++) {
			fprintf(fp, "%f\n", record[i]);
		}

		if (fclose(fp)) {
			initConsole();
			printf("�޷��ر�record.txt�ļ���\n");
		}
	}
	else {
		initConsole();
		printf("�Ҳ���record.txt�ļ���\n");
		state = END;
	}
}