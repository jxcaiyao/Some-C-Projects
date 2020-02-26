#ifndef __MODEL__
#define __MODEL__

#define INTERVAL 10			//定时器刷新周期、输出周期（单位：ms）

#define PI 3.1415926535897

#define BATTERYX (WIDTH/2)	//初始堡垒x位置
#define BATTERYY (HEIGHT/2)	//初始堡垒y位置
#define BATTERYB 100		//初始堡垒血量
#define AWARDNUM 4

typedef struct _point {		//坐标
	double x;
	double y;
} Point;

typedef struct _velocity {	//速度
	double vx;
	double vy;
} Velocity;

typedef struct _plane {		//飞机
	Point p;
	Velocity v;
	struct _plane *last;
	struct _plane *next;
} Plane;

typedef struct _planeList {	//飞机链表
	Plane *head;
	Plane *tail;
} planeList;

typedef struct _bomb {		//导弹
	Point p;
	Velocity v;
	struct _bomb *last;
	struct _bomb *next;
} Bomb;

typedef struct _bombList {	//导弹链表
	Bomb *head;
	Bomb *tail;
} bombList;

typedef struct _battery {	//堡垒
	Point p;
	int blood;				//血量
	int type0;				//0型补给使用次数
} Battery;

typedef struct _bullet {	//炮弹
	Point p;
	Velocity v;
	struct _bullet *last;
	struct _bullet *next;
} Bullet;

typedef struct _bulletList {//炮弹链表
	Bullet *head;
	Bullet *tail;
} bulletList;

typedef struct _crash {		//爆炸
	Point p;
	int time;				//持续时间
	int radius;				//爆炸半径
	struct _crash *last;
	struct _crash *next;
} Crash;

typedef struct _crashList {	//爆炸链表
	Crash *head;
	Crash *tail;
} crashList;

typedef struct _limit {		//表示某一数据的取值范围
	double FLOOR;
	double CEIL;
} LIMIT;

typedef struct _award {		//补给品
	int time;				//存在时间
	int type;				//类型
	Point p;
	struct _award *last;
	struct _award *next;
} Award;

typedef struct _awardList {	//补给品链表
	Award *head;
	Award *tail;
} awardList;

typedef struct _allObject {	//所有游戏元素
	planeList *pll;
	bombList *bol;
	bulletList *bul;
	crashList *crl;
	awardList *awl;
	Battery *bat;
} allObject;

enum STATEOFGAME { START, PROCESS, END };			//游戏所处阶段
enum DIFFICULTYOFGAME { EASY, MEDIUM, HARD, DOOM };	//游戏难度

extern allObject allObj;					//所有游戏元素
extern int state;							//游戏所处阶段
extern double gameTime;						//游戏时间
extern enum DIFFICULTYOFGAME difficulty;	//游戏难度

extern double record[4];					//四种难度的游戏记录

int random(int m, int n);		//给定随机数上下限，随机出一个整数
int sgn(double x);				//取符号函数

void addPlane(planeList *pList);					//增加一架飞机
void changePlane(planeList *pList);					//改变飞机位置
void deletePlane(planeList *pList, Plane **pl);		//删除链表中的某架飞机
void freePlane(planeList *pList);					//删除整个飞机链表

void addBomb(bombList *bList, Point p);				//增加一个导弹
void changeBomb(bombList *bList);					//改变所有导弹位置
void deleteBomb(bombList *bList, Bomb **bo);		//删除某个导弹
void freeBomb(bombList *bList);						//删除整个导弹链表

void addBullet(bulletList *bList, double angle);	//增加一个炮弹，以angle角射出
void changeBullet(bulletList *bList);				//改变所有炮弹位置
void deleteBullet(bulletList *bList, Bullet **bu);	//删除一个炮弹
void freeBullet(bulletList *bList);					//删除整个炮弹链表

void addCrash(crashList *cList, Point p, int radius);	//在p位置增加一个半径为radius的爆炸
void changeCrash(crashList *cList);						//减少所有爆炸存在时间，使爆炸定时消失
void deleteCrash(crashList *cList, Crash **cr);			//删除某个爆炸
void freeCrash(crashList *cList);						//删除整个爆炸链表

void addAward(awardList *aList, Point p, int type);	//增加一个补给（奖励）
void changeAward(awardList *aList);					//减少补给存在时间
void deleteAward(awardList *aList, Award **aw);		//删除某个补给
void freeAward(awardList *aList);					//删除整个补给链表

void changeBattery(Battery *ba);					//改变堡垒属性
void reductBattery(Battery *ba, int damage);		//减少堡垒血量

void initAll(allObject *allObj);					//初始化所有游戏元素
void setDifficulty(int df);							//根据选择游戏难度设定参数
void changeAll(allObject *allObj);					//将所有改变游戏元素的函数整合到一起
void GameOver(void);								//游戏结束

int probability(double p);							//返回1的概率为p,返回0的概率为1-p
void randPlane(planeList *pList);					//每隔一段时间生成一架飞机
void randBomb(bombList *bList, planeList *pList);	//每隔一段时间随机生成一次导弹
void randAward(awardList *aList);					//每隔一段时间随机生成一次补给品

void decideCrash(allObject *allObj);									//判断碰撞是否发生
void PlBuCrash(planeList *pList, bulletList *bList, crashList *cList);	//飞机与炮弹碰撞
int PlBuCrashCondi(Plane *pl, Bullet *bu);								//飞机与炮弹碰撞返回1，未碰撞返回0
void BoBuCrash(bombList *boList, bulletList *buList, crashList *crList);//导弹与炮弹碰撞
int BoBuCrashCondi(Bomb *bo, Bullet *bu);								//导弹与炮弹碰撞返回1，未碰撞返回0
void BoBaCrash(bombList *boList, Battery *ba, crashList *crList);		//导弹与堡垒碰撞
int BoBaCrashCondi(Bomb *bo, Battery *ba);								//导弹与堡垒碰撞返回1，未碰撞返回0
void BoAwCrash(bombList *boList, awardList *awList, crashList *crList);
int BoAwCrashCondi(Bomb *bo, Award *aw);
void AwBaCrash(awardList *awList, Battery *ba);							//补给与堡垒碰撞
int AwBaCrashCondi(Award *aw, Battery *ba);								//补给与堡垒碰撞返回1，未碰撞返回0

void getRecord(void);		//得到文件中的游戏记录
void putRecord(void);		//将新游戏记录写入文档

#endif