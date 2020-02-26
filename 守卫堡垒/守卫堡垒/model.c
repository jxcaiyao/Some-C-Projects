#include"all.h"

const LIMIT PLANEX = { 0, 1 };				//0表示飞机从左出来，1表示飞机从右出来
const LIMIT PLANEY = { HEIGHT / 10, 4 * HEIGHT / 10 };	//飞机在60到240的高度随机刷出
const LIMIT PLANEVX = { WIDTH / 12, WIDTH / 8 };		//飞机水平飞行速度范围为67到100
const LIMIT PLANEVY = { 0, 0 };							//飞机垂直飞行速度为0

const int bombv[4] = { 200, 200, 200, 400 };			//四种难度下导弹的不同飞行速度
int BOMBV;

const int bombd[4] = { 5, 10, 20, 30 };					//四种难度下导弹的威力
int BOMBD;

const int bulletv[4] = { 400, 400, 600, 800 };			//四种难度下炮弹的速度
int BULLETV;

const int CRASHT = 500;									//爆炸持续时间为0.5s

const int planeperiod[4] = { 1000, 1000, 500, 250 };	//四种难度下飞机刷新的周期，（单位：ms）
int PLANEPERIOD;

const double bombprob[4] = { 0.5,0.6,0.7,0.8 };			//四种难度下导弹出现的概率
double BOMBPROB;

const int bombperiod[4] = { 1000, 1000, 750, 500 };		//四种难度下 判断导弹是否刷新 的周期
int BOMBPERIOD;

const int awardperiod[AWARDNUM][4] = {	{ 3000, 2000, 1000, 500 },
										{ 10000, 8000, 6000, 3000 },
										{ 5000, 4000, 3000, 2000 },
										{ 5000, 4000, 3000, 2000 } };	//四种难度下 判断补给是否刷新 的周期
int AWARDPERIOD[AWARDNUM];

const int AWARDT[AWARDNUM] = { 5000, 2000, 3000, 2000 };
const double AWARDPROB[AWARDNUM] = { 0.8, 0.7, 0.5, 0.5 };

const LIMIT AWARDPX[AWARDNUM] = { { 100, 700 }, { 200, 600 }, { 200, 600 }, { 100, 700 } };
const LIMIT AWARDPY[AWARDNUM] = { { 200, 550 }, { 300, 450 }, { 300, 400 }, { 200, 550 } };

allObject allObj;						//包含所有游戏元素
int state;								//游戏状态
double gameTime;						//一局游戏进行的时间
enum DIFFICULTYOFGAME difficulty;		//游戏难度

double record[4] = { 0 };				//记录四种难度的最好成绩

int random(int m, int n)				//给定随机数上下限，随机出一个整数
{
	int randnum;
	static int i;
	srand(++i);							//使用自增数作为种子可以避免在极短时间内随机数都相同
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

int sgn(double x)						//取符号函数
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

void addPlane(planeList *pList)			//增加一架飞机
{
	Plane *pl;
	pl = (Plane*)malloc(sizeof(Plane));
	pl->p.x = random((int)PLANEX.FLOOR, (int)PLANEX.CEIL) * WIDTH;
	pl->p.y = random((int)PLANEY.FLOOR, (int)PLANEY.CEIL);
	pl->v.vx = random((int)PLANEVX.FLOOR, (int)PLANEVX.CEIL);
	if (pl->p.x > 0) {
		pl->v.vx = -pl->v.vx;			//飞机从右边出来时注意水平速度是负的
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

void changePlane(planeList *pList)		//改变飞机位置
{
	Plane *pl = pList->head;
	while (pl) {
		pl->p.x += pl->v.vx * INTERVAL / 1000;
		pl->p.y += pl->v.vy * INTERVAL / 1000;
		if (pl->p.x > WIDTH || pl->p.x < 0) {
			deletePlane(pList, &pl);	//如果飞出窗口则删除此架飞机
		}
		else {
			pl = pl->next;
		}
	}
}

void deletePlane(planeList *pList, Plane **pl)	//删除链表中的某架飞机
{
	if ((*pl)->last) {							//如果不是链表中的头一个
		if ((*pl)->next) {						//如果不是链表中的最后一个
			(*pl)->last->next = (*pl)->next;
			(*pl)->next->last = (*pl)->last;
		}
		else {									//如果是链表中的最后一个
			(*pl)->last->next = (*pl)->next;
			pList->tail = (*pl)->last;
		}
	}
	else if((*pl)->next){						//如果是链表中的头一个而且不是最后一个
		(*pl)->next->last = (*pl)->last;
		pList->head = (*pl)->next;
	}
	else {										//既是链表的头一个，也是链表的最后一个
		pList->head = pList->tail = NULL;
	}
	Plane *temp;
	temp = (*pl)->next;
	free(*pl);
	*pl = temp;
}

void freePlane(planeList *pList)				//删除整个飞机链表
{
	Plane *p, *q;
	for (p = pList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addBomb(bombList *bList, Point p)			//增加一个导弹
{
	Bomb *bo;
	bo = (Bomb*)malloc(sizeof(Bomb));
	bo->p.x = p.x;
	bo->p.y = p.y;
	
	double a;
	a = atan((allObj.bat->p.x - (double)p.x) / (allObj.bat->p.y - (double)p.y));
	bo->v.vx = /*(int)*/sgn(allObj.bat->p.y - (double)p.y) * (BOMBV * sin(a));		//导弹指向堡垒
	bo->v.vy = /*(int)*/sgn(allObj.bat->p.y - (double)p.y) * (BOMBV * cos(a));		
	//注意因为atan的取值范围问题所以需要取符号函数的辅助
	
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

void changeBomb(bombList *bList)				//改变所有导弹位置
{
	Bomb *bo = bList->head;
	while (bo) {
		bo->p.x += bo->v.vx * INTERVAL / 1000 * (0.1 * gameTime + 1);	//导弹速度随游戏时间线性增加
		bo->p.y += bo->v.vy * INTERVAL / 1000 * (0.1 * gameTime + 1);
		//double a;			//此段注释可使导弹追踪堡垒
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

void deleteBomb(bombList *bList, Bomb **bo)			//删除某个导弹
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

void freeBomb(bombList *bList)				//删除整个导弹链表
{
	Bomb *p, *q;
	for (p = bList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addBullet(bulletList *bList, double angle)		//增加一个炮弹，以angle角射出
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

void changeBullet(bulletList *bList)				//改变所有炮弹位置
{
	Bullet *bu = bList->head;
	//Bomb *bo = allObj.bol->head;
	while (bu) {
		bu->p.x += bu->v.vx * INTERVAL / 1000;
		bu->p.y += bu->v.vy * INTERVAL / 1000;
		//double a;				//此段注释可使炮弹追踪导弹（不好用）
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

void deleteBullet(bulletList *bList, Bullet **bu)		//删除一个炮弹
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

void freeBullet(bulletList *bList)			//删除整个炮弹链表
{
	Bullet *p, *q;
	for (p = bList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addCrash(crashList *cList, Point p, int radius)	//在p位置增加一个半径为radius的爆炸
{
	Crash *cr;
	cr = (Crash*)malloc(sizeof(Crash));
	cr->p.x = p.x;
	cr->p.y = p.y;
	cr->time = CRASHT;				//爆炸效果持续时间
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

void changeCrash(crashList *cList)			//减少所有爆炸存在时间，使爆炸定时消失
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

void deleteCrash(crashList *cList, Crash **cr)		//删除某个爆炸
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

void freeCrash(crashList *cList)			//删除整个爆炸链表
{
	Crash *p, *q;
	for (p = cList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void addAward(awardList *aList, Point p, int type)	//增加一个补给（奖励）
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

void changeAward(awardList *aList)		//减少补给存在时间
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

void deleteAward(awardList *aList, Award **aw)		//删除某个补给
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
void freeAward(awardList *aList)		//删除整个补给链表
{
	Award *p, *q;
	for (p = aList->head; p; p = q) {
		q = p->next;
		free(p);
	}
}

void changeBattery(Battery *ba)			//改变堡垒属性
{
	if (ba->type0) {					//使用0型补给后螺旋释放炮弹
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180);
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180 + 2 * PI / 3);
		addBullet(allObj.bul, ba->type0 * 21 * PI / 180 + 4 * PI / 3);
		ba->type0 --;
	}
}

void reductBattery(Battery *ba, int damage)		//减少堡垒血量
{
	if (ba->blood - damage > 0) {
		ba->blood -= damage;
	}
	else {										//堡垒血掉光了
		ba->blood = 0;
		addCrash(allObj.crl, ba->p, 35);		//堡垒爆炸
		GameOver();								//游戏结束
	}
}

void initAll(allObject *allObj)					//初始化所有游戏元素
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
	
	bat->p.x = BATTERYX;		//堡垒初始化
	bat->p.y = BATTERYY;
	bat->blood = BATTERYB;
	bat->type0 = 0;				//0型补给可使用次数为0

	getRecord();				//从文档中得到以往游戏记录最高分

	state = START;				//游戏开始

	gameTime = 0;				//游戏时间初始化为0
}

void setDifficulty(int df)		//根据选择游戏难度设定参数
{
	initAll(&allObj);			//初始化所有游戏元素

	//stopSound(0);
	//stopSound(2);
	//playSound(1, 1);

	BOMBV = bombv[df];				//导弹速度
	BOMBD = bombd[df];				//导弹伤害
	BULLETV = bulletv[df];			//炮弹速度
	BOMBPROB = bombprob[df];		//导弹生成可能性
	PLANEPERIOD = planeperiod[df];	//飞机生成周期
	BOMBPERIOD = bombperiod[df];	//导弹生成判定周期

	int i;
	for (i = 0; i < AWARDNUM; i++) {
		AWARDPERIOD[i] = awardperiod[i][df];	//补给生成判定周期
	}

	difficulty = df;				//存储游戏难度

	state = PROCESS;				//游戏处于进行阶段
}

void changeAll(allObject *allObj)		//将所有改变游戏元素的函数整合到一起
{
	randPlane(allObj->pll);				//生成飞机
	randBomb(allObj->bol, allObj->pll);	//随机生成导弹
	randAward(allObj->awl);				//随机生成补给品

	changePlane(allObj->pll);
	changeBomb(allObj->bol);
	changeBullet(allObj->bul);
	changeCrash(allObj->crl);
	changeAward(allObj->awl);
	changeBattery(allObj->bat);
}

void GameOver(void)				//游戏结束
{
	state = END;				//游戏进入结束阶段
	//stopSound(1);
	//playSound(2, 1);
	putRecord();				//将游戏成绩存入文档
}

int probability(double p)		//返回1的概率为p,返回0的概率为1-p
{
	int ret = 0;
	if (random(0, 1000) <= 1000 * p) {
		ret = 1;
	}
	return ret;
}

void randPlane(planeList *pList)	//每隔一段时间生成一架飞机
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

void randBomb(bombList *bList, planeList *pList)	//每隔一段时间随机生成一次导弹
{
	Plane *pl = pList->head;
	static int i = 0;
	if (i <= 0) {
		while (pl) {
			if (probability(BOMBPROB * (0.005 * gameTime + 1))) {
				addBomb(bList, pl->p);
				i = BOMBPERIOD * exp(-gameTime * 0.05) / INTERVAL;	//导弹生成周期随游戏时间逐渐缩短
			}
			pl = pl->next;
		}
	}
	else {
		i--;
	}
}

void randAward(awardList *aList)	//每隔一段时间随机生成一次补给品
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
				p.x = random(AWARDPX[i].FLOOR, AWARDPX[i].CEIL);			//补给品随机生成位置
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

void decideCrash(allObject *allObj)						//判断碰撞是否发生
{
	PlBuCrash(allObj->pll, allObj->bul, allObj->crl);	//飞机与炮弹碰撞
	BoBuCrash(allObj->bol, allObj->bul, allObj->crl);	//导弹与炮弹碰撞
	BoBaCrash(allObj->bol, allObj->bat, allObj->crl);	//导弹与堡垒碰撞
	BoAwCrash(allObj->bol, allObj->awl, allObj->crl);	//导弹与补给碰撞
	AwBaCrash(allObj->awl, allObj->bat);				//补给与堡垒碰撞
}

void PlBuCrash(planeList *pList, bulletList *bList, crashList *cList)	//飞机与炮弹碰撞
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

int PlBuCrashCondi(Plane *pl, Bullet *bu)		//飞机与炮弹碰撞返回1，未碰撞返回0
{
	int ret = 0;
	if (sqrt(pow(pl->p.x - bu->p.x, 2) + pow(pl->p.y - bu->p.y, 2)) <= 20) {	//相距小于20
		ret = 1;
	}
	return ret;
}

void BoBuCrash(bombList *boList, bulletList *buList, crashList *crList)	//导弹与炮弹碰撞
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

int BoBuCrashCondi(Bomb *bo, Bullet *bu)		//导弹与炮弹碰撞返回1，未碰撞返回0
{
	int ret = 0;
	if (sqrt(pow(bo->p.x - bu->p.x, 2) + pow(bo->p.y - bu->p.y, 2)) <= 20) {	//相距小于20
		ret = 1;
	}
	return ret;
}

void BoBaCrash(bombList *boList, Battery *ba, crashList *crList)	//导弹与堡垒碰撞
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

int BoBaCrashCondi(Bomb *bo, Battery *ba)		//导弹与堡垒碰撞返回1，未碰撞返回0
{
	int ret = 0;
	if (sqrt(pow(bo->p.x - ba->p.x, 2) + pow(bo->p.y - ba->p.y, 2)) <= 40) {	//相距小于40
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

void AwBaCrash(awardList *awList, Battery *ba)	//补给品与堡垒碰撞
{
	Award *aw = awList->head;
	Plane *pl = allObj.pll->head;
	Bomb *bo = allObj.bol->head;

	while (aw) {
		if (AwBaCrashCondi(aw, ba)) {
			switch (aw->type) {
			case 0:
				ba->type0 += 500 / INTERVAL;		//堡垒可获得0.5秒加成
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

int AwBaCrashCondi(Award *aw, Battery *ba)		//补给品与堡垒碰撞返回1，未碰撞返回0
{
	int ret = 0;

	switch (aw->type) {
	case 0:
	case 1:
	case 3:
		if (sqrt(pow(aw->p.x - ba->p.x, 2) + pow(aw->p.y - ba->p.y, 2)) <= 45) {	//相距小于45
			ret = 1;
		}
		break;
	default:
		break;
	}

	return ret;
}

void getRecord(void)				//得到文件中的游戏记录
{
	FILE *fp;

	if (!fopen_s(&fp, "record.txt", "r")) {	//打开文件成功
		int i;

		for (i = 0; i < 4; i++) {
			fscanf_s(fp, "%lf", &record[i]);
		}

		if (fclose(fp)) {
			initConsole();
			printf("无法关闭record.txt文件！\n");
		}
	}
	else {					//打开文件失败
		//initConsole();
		//printf("找不到record.txt文件！\n");
		//state = END;
		fp = fopen("record.txt", "w+");		//新建该文件
		fclose(fp);
		getRecord();						//重新读取文件中的记录
	}
}

void putRecord(void)			//将新游戏记录写入文档
{
	FILE *fp;

	if (!fopen_s(&fp, "record.txt", "w")) {
		int i;

		if (gameTime > record[difficulty]) {	//如果打破了游戏记录则将其记为新的记录
			record[difficulty] = gameTime;
		}

		for (i = 0; i < 4; i++) {
			fprintf(fp, "%f\n", record[i]);
		}

		if (fclose(fp)) {
			initConsole();
			printf("无法关闭record.txt文件！\n");
		}
	}
	else {
		initConsole();
		printf("找不到record.txt文件！\n");
		state = END;
	}
}