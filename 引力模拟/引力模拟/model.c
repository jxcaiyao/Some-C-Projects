#include"All.h"

CelBdy Stars[NUM];

Queue QStars[NUM];

Vector CoSys[4] = {
	{-128, -128, -128},
	{128, -128, -128},
	{-128, 128, -128},
	{-128, -128, 128}
};

VisAng Angle = { 0, PI/2 };

const double G = 6.67e-11;
const int INTERVAL = 10;
const int TIMERATIO = 2e5 / 1e3 * 10;
const int DISTRATIO = 2e6;

void initStars(void)
{
	Stars[0].mass = 5.97e24;
	Stars[0].radius = 6.37e6;

	Stars[0].P.x = 0;
	Stars[0].P.y = 0;
	Stars[0].P.z = 0;

	Stars[0].V.x = -4.93/*-12.6*/;
	Stars[0].V.y = 0;
	Stars[0].V.z = 0;

	Stars[1].mass = 7.35e22;
	Stars[1].radius = 1.23e6/*1.74e6*/;

	Stars[1].P.x = 0;
	Stars[1].P.y = 3.84e8;
	Stars[1].P.z = 0;

	Stars[1].V.x = 1.02e3/*1.02e3*/;
	Stars[1].V.y = 0;
	Stars[1].V.z = 0;

	Stars[2].mass = 4.53e22/*0*/;
	Stars[2].radius = 1.76e6/*0*/;

	Stars[2].P.x = 0;
	Stars[2].P.y = -5.02e8;
	Stars[2].P.z = 0;

	Stars[2].V.x = -8e2;
	Stars[2].V.y = 0;
	Stars[2].V.z = 0;

	//Stars[0].mass = 5e23;
	//Stars[0].radius = 5e6;
	//Stars[0].P.x = -5e8;
	//Stars[0].V.y = 3.4e1;

	//Stars[1].mass = 5e23;
	//Stars[1].radius = 5e6;
	//Stars[1].P.x = 2.5e8;
	//Stars[1].P.y = 4.33e8;
	//Stars[1].V.x = 2.94e1;
	//Stars[1].V.y = -1.96e1;

	//Stars[2].mass = 5e23;
	//Stars[2].radius = 5e6;
	//Stars[2].P.x = 2.5e8;
	//Stars[2].P.y = -4.33e8;
	//Stars[2].V.x = -2.94e1;
	//Stars[2].V.y = -1.96e1;
}

void TimerEvent(int tid)
{
	Display(Angle, Stars, NUM);
	Record(Stars, QStars, NUM);
	ChangeAll(Stars, NUM);
	Record(Stars, QStars, NUM);
	ChangeAll(Stars, NUM);
	Record(Stars, QStars, NUM);
	ChangeAll(Stars, NUM);
}

void ChangeAll(CelBdy Stars[], int num)
{
	CalAcl(Stars, num);
	CalVel(Stars, num);
	CalPot(Stars, num);
}

void CalAcl(CelBdy Stars[], int num)
{
	int i, j;

	double R;
	double A;

	for (i = 0; i < num; i++) {

		Stars[i].A.x = 0;
		Stars[i].A.y = 0;
		Stars[i].A.z = 0;

		for (j = 0; j < num; j++) {
			if (i == j)
				continue;

			R = sqrt(pow(Stars[i].P.x - Stars[j].P.x, 2) + pow(Stars[i].P.y - Stars[j].P.y, 2) + pow(Stars[i].P.z - Stars[j].P.z, 2));
			A = G * Stars[j].mass / pow(R, 2);

			Stars[i].A.x += A * (Stars[j].P.x - Stars[i].P.x) / R;
			Stars[i].A.y += A * (Stars[j].P.y - Stars[i].P.y) / R;
			Stars[i].A.z += A * (Stars[j].P.z - Stars[i].P.z) / R;
		}
	}
}

void CalVel(CelBdy Stars[], int num)
{
	int i;
	for (i = 0; i < num; i++) {
		Stars[i].V.x += Stars[i].A.x * TIMERATIO;
		Stars[i].V.y += Stars[i].A.y * TIMERATIO;
		Stars[i].V.z += Stars[i].A.z * TIMERATIO;
	}
}

void CalPot(CelBdy Stars[], int num)
{
	int i;
	for (i = 0; i < num; i++) {
		Stars[i].P.x += Stars[i].V.x * TIMERATIO;
		Stars[i].P.y += Stars[i].V.y * TIMERATIO;
		Stars[i].P.z += Stars[i].V.z * TIMERATIO;
	}
}

void initQStars(Queue QStars[], int num)
{
	int i;
	for (i = 0; i < num; i++) {
		QStars[i] = CreateQueue();
	}
}

Queue CreateQueue(void)
{
	Queue Q = (Queue)malloc(sizeof(struct QNode));
	Q->front = 0;
	Q->rear = 0;
	return Q;
}

void AddQ(Queue PtrQ, Vector item)
{
	if (IsFullQ(PtrQ)) {
		DeleteQ(PtrQ);
		return;
	}
	PtrQ->rear = (PtrQ->rear + 1) % MaxSize;

	PtrQ->Data[PtrQ->rear].x = item.x - Stars[0].P.x;
	PtrQ->Data[PtrQ->rear].y = item.y - Stars[0].P.y;
	PtrQ->Data[PtrQ->rear].z = item.z - Stars[0].P.z;

	//PtrQ->Data[PtrQ->rear] = item;
}

int IsFullQ(Queue PtrQ)
{
	return (PtrQ->rear + 1) % MaxSize == PtrQ->front;
}

Vector DeleteQ(Queue PtrQ)
{
	if (IsEmptyQ(PtrQ)) {
		//printf("╤сап©у\n");
	}
	else {
		PtrQ->front = (PtrQ->front + 1) % MaxSize;
		return PtrQ->Data[PtrQ->front];
	}
}

int IsEmptyQ(Queue PtrQ)
{
	return PtrQ->front == PtrQ->rear;
}

void Record(CelBdy Stars[], Queue QStars[], int num)
{
	int i;
	for (i = 0; i < num; i++) {
		AddQ(QStars[i], Stars[i].P);
	}
}