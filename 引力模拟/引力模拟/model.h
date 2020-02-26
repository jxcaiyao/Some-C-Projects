#pragma once

#define PI 3.1415927
#define NUM 3
#define MaxSize 800

typedef struct _VisAng {
	double a;
	double b;
} VisAng;

typedef struct _Vector {
	double x;
	double y;
	double z;
} Vector;

typedef struct _CelBdy {
	double mass;
	double radius;
	Vector P;
	Vector V;
	Vector A;
} CelBdy;

struct QNode {
	Vector Data[MaxSize];
	int rear;
	int front;
};
typedef struct QNode *Queue;

extern CelBdy Stars[NUM];
extern Queue QStars[NUM];
extern VisAng Angle;
extern Vector CoSys[4];

extern const double G;
extern const int INTERVAL;
extern const int TIMERATIO;
extern const int DISTRATIO;

void initStars(void);
void TimerEvent(int tid);
void ChangeAll(CelBdy Stars[], int num);
void CalAcl(CelBdy Stars[], int num);
void CalVel(CelBdy Stars[], int num);
void CalPot(CelBdy Stars[], int num);

void initQStars(Queue QStars[], int num);
Queue CreateQueue(void);
void AddQ(Queue PtrQ, Vector item);
int IsFullQ(Queue PtrQ);
Vector DeleteQ(Queue PtrQ);
int IsEmptyQ(Queue PtrQ);
void Record(CelBdy Stars[], Queue QStars[], int num);