#ifndef __MODEL__
#define __MODEL__

#define INTERVAL 10			//��ʱ��ˢ�����ڡ�������ڣ���λ��ms��

#define PI 3.1415926535897

#define BATTERYX (WIDTH/2)	//��ʼ����xλ��
#define BATTERYY (HEIGHT/2)	//��ʼ����yλ��
#define BATTERYB 100		//��ʼ����Ѫ��
#define AWARDNUM 4

typedef struct _point {		//����
	double x;
	double y;
} Point;

typedef struct _velocity {	//�ٶ�
	double vx;
	double vy;
} Velocity;

typedef struct _plane {		//�ɻ�
	Point p;
	Velocity v;
	struct _plane *last;
	struct _plane *next;
} Plane;

typedef struct _planeList {	//�ɻ�����
	Plane *head;
	Plane *tail;
} planeList;

typedef struct _bomb {		//����
	Point p;
	Velocity v;
	struct _bomb *last;
	struct _bomb *next;
} Bomb;

typedef struct _bombList {	//��������
	Bomb *head;
	Bomb *tail;
} bombList;

typedef struct _battery {	//����
	Point p;
	int blood;				//Ѫ��
	int type0;				//0�Ͳ���ʹ�ô���
} Battery;

typedef struct _bullet {	//�ڵ�
	Point p;
	Velocity v;
	struct _bullet *last;
	struct _bullet *next;
} Bullet;

typedef struct _bulletList {//�ڵ�����
	Bullet *head;
	Bullet *tail;
} bulletList;

typedef struct _crash {		//��ը
	Point p;
	int time;				//����ʱ��
	int radius;				//��ը�뾶
	struct _crash *last;
	struct _crash *next;
} Crash;

typedef struct _crashList {	//��ը����
	Crash *head;
	Crash *tail;
} crashList;

typedef struct _limit {		//��ʾĳһ���ݵ�ȡֵ��Χ
	double FLOOR;
	double CEIL;
} LIMIT;

typedef struct _award {		//����Ʒ
	int time;				//����ʱ��
	int type;				//����
	Point p;
	struct _award *last;
	struct _award *next;
} Award;

typedef struct _awardList {	//����Ʒ����
	Award *head;
	Award *tail;
} awardList;

typedef struct _allObject {	//������ϷԪ��
	planeList *pll;
	bombList *bol;
	bulletList *bul;
	crashList *crl;
	awardList *awl;
	Battery *bat;
} allObject;

enum STATEOFGAME { START, PROCESS, END };			//��Ϸ�����׶�
enum DIFFICULTYOFGAME { EASY, MEDIUM, HARD, DOOM };	//��Ϸ�Ѷ�

extern allObject allObj;					//������ϷԪ��
extern int state;							//��Ϸ�����׶�
extern double gameTime;						//��Ϸʱ��
extern enum DIFFICULTYOFGAME difficulty;	//��Ϸ�Ѷ�

extern double record[4];					//�����Ѷȵ���Ϸ��¼

int random(int m, int n);		//��������������ޣ������һ������
int sgn(double x);				//ȡ���ź���

void addPlane(planeList *pList);					//����һ�ܷɻ�
void changePlane(planeList *pList);					//�ı�ɻ�λ��
void deletePlane(planeList *pList, Plane **pl);		//ɾ�������е�ĳ�ܷɻ�
void freePlane(planeList *pList);					//ɾ�������ɻ�����

void addBomb(bombList *bList, Point p);				//����һ������
void changeBomb(bombList *bList);					//�ı����е���λ��
void deleteBomb(bombList *bList, Bomb **bo);		//ɾ��ĳ������
void freeBomb(bombList *bList);						//ɾ��������������

void addBullet(bulletList *bList, double angle);	//����һ���ڵ�����angle�����
void changeBullet(bulletList *bList);				//�ı������ڵ�λ��
void deleteBullet(bulletList *bList, Bullet **bu);	//ɾ��һ���ڵ�
void freeBullet(bulletList *bList);					//ɾ�������ڵ�����

void addCrash(crashList *cList, Point p, int radius);	//��pλ������һ���뾶Ϊradius�ı�ը
void changeCrash(crashList *cList);						//�������б�ը����ʱ�䣬ʹ��ը��ʱ��ʧ
void deleteCrash(crashList *cList, Crash **cr);			//ɾ��ĳ����ը
void freeCrash(crashList *cList);						//ɾ��������ը����

void addAward(awardList *aList, Point p, int type);	//����һ��������������
void changeAward(awardList *aList);					//���ٲ�������ʱ��
void deleteAward(awardList *aList, Award **aw);		//ɾ��ĳ������
void freeAward(awardList *aList);					//ɾ��������������

void changeBattery(Battery *ba);					//�ı䱤������
void reductBattery(Battery *ba, int damage);		//���ٱ���Ѫ��

void initAll(allObject *allObj);					//��ʼ��������ϷԪ��
void setDifficulty(int df);							//����ѡ����Ϸ�Ѷ��趨����
void changeAll(allObject *allObj);					//�����иı���ϷԪ�صĺ������ϵ�һ��
void GameOver(void);								//��Ϸ����

int probability(double p);							//����1�ĸ���Ϊp,����0�ĸ���Ϊ1-p
void randPlane(planeList *pList);					//ÿ��һ��ʱ������һ�ܷɻ�
void randBomb(bombList *bList, planeList *pList);	//ÿ��һ��ʱ���������һ�ε���
void randAward(awardList *aList);					//ÿ��һ��ʱ���������һ�β���Ʒ

void decideCrash(allObject *allObj);									//�ж���ײ�Ƿ���
void PlBuCrash(planeList *pList, bulletList *bList, crashList *cList);	//�ɻ����ڵ���ײ
int PlBuCrashCondi(Plane *pl, Bullet *bu);								//�ɻ����ڵ���ײ����1��δ��ײ����0
void BoBuCrash(bombList *boList, bulletList *buList, crashList *crList);//�������ڵ���ײ
int BoBuCrashCondi(Bomb *bo, Bullet *bu);								//�������ڵ���ײ����1��δ��ײ����0
void BoBaCrash(bombList *boList, Battery *ba, crashList *crList);		//�����뱤����ײ
int BoBaCrashCondi(Bomb *bo, Battery *ba);								//�����뱤����ײ����1��δ��ײ����0
void BoAwCrash(bombList *boList, awardList *awList, crashList *crList);
int BoAwCrashCondi(Bomb *bo, Award *aw);
void AwBaCrash(awardList *awList, Battery *ba);							//�����뱤����ײ
int AwBaCrashCondi(Award *aw, Battery *ba);								//�����뱤����ײ����1��δ��ײ����0

void getRecord(void);		//�õ��ļ��е���Ϸ��¼
void putRecord(void);		//������Ϸ��¼д���ĵ�

#endif