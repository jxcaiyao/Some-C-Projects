#include"All.h"

void Display(VisAng Angle, CelBdy Stars[], int num)
{
	beginPaint();

	clearDevice();
	setPenColor(BLACK);
	line(0, HEIGHT / 2, WIDTH, HEIGHT / 2);
	line(WIDTH / 2, 0, WIDTH / 2, HEIGHT);

	setPenColor(RED);
	DisplayLine(Angle, CoSys[0], CoSys[1]);
	setPenColor(GREEN);
	DisplayLine(Angle, CoSys[0], CoSys[2]);
	setPenColor(BLUE);
	DisplayLine(Angle, CoSys[0], CoSys[3]);

	setPenColor(BLACK);
	int i;
	for (i = 0; i < num; i++) {
		DisplayBody(Angle, Stars[i]);
		DisplayOrbit(Angle, QStars[i], i);
	}


	endPaint();
}

void DisplayBody(VisAng Angle, CelBdy Star)
{
	double x, y;

	x = -sin(Angle.a) * Star.P.x + cos(Angle.a) * Star.P.y;
	y = -cos(Angle.a) * sin(Angle.b) * Star.P.x - sin(Angle.a) * sin(Angle.b) * Star.P.y + cos(Angle.b) * Star.P.z;

	//ellipse(CoTranX(x - Star.radius), CoTranY(y + Star.radius), CoTranX(x + Star.radius), CoTranY(y - Star.radius));
	double x1, y1;
	x1 = -sin(Angle.a) * Stars[0].P.x + cos(Angle.a) * Stars[0].P.y;
	y1 = -cos(Angle.a) * sin(Angle.b) * Stars[0].P.x - sin(Angle.a) * sin(Angle.b) * Stars[0].P.y + cos(Angle.b) * Stars[0].P.z;
	ellipse(CoTranX(x - x1 - Star.radius), CoTranY(y - y1 + Star.radius), CoTranX(x - x1 + Star.radius), CoTranY(y - y1 - Star.radius));
}

void DisplayLine(VisAng Angle, Vector p1, Vector p2)
{
	double x1, y1, x2, y2;

	x1 = -sin(Angle.a) * p1.x + cos(Angle.a) * p1.y;
	y1 = -cos(Angle.a) * sin(Angle.b) * p1.x - sin(Angle.a) * sin(Angle.b) * p1.y + cos(Angle.b) * p1.z;

	x2 = -sin(Angle.a) * p2.x + cos(Angle.a) * p2.y;
	y2 = -cos(Angle.a) * sin(Angle.b) * p2.x - sin(Angle.a) * sin(Angle.b) * p2.y + cos(Angle.b) * p2.z;

	line(WIDTH/2 + x1, HEIGHT/2 - y1, WIDTH / 2 + x2, HEIGHT / 2 - y2);
}

void DisplayOrbit(VisAng Angle, Queue PtrQ, int index)
{
	double x, y;
	int i;

	if (IsEmptyQ(PtrQ)) 
		return;

	for (i = PtrQ->front; i != PtrQ->rear; i = (i+1) % MaxSize) {
		x = -sin(Angle.a) * PtrQ->Data[i].x + cos(Angle.a) * PtrQ->Data[i].y;
		y = -cos(Angle.a) * sin(Angle.b) * PtrQ->Data[i].x - sin(Angle.a) * sin(Angle.b) * PtrQ->Data[i].y + cos(Angle.b) * PtrQ->Data[i].z;
		switch (index) {
		case 0:	
			putPixel(CoTranX(x), CoTranY(y), BLACK);
			break;
		case 1:
			putPixel(CoTranX(x), CoTranY(y), RED);
			break;
		case 2:
			putPixel(CoTranX(x), CoTranY(y), BLUE);
			break;
		}
	}
}

int CoTranX(double x)
{
	return WIDTH / 2 + x / DISTRATIO;
}

int CoTranY(double y)
{
	return HEIGHT / 2 - y / DISTRATIO;
}

//
//void DisplayPixel(VisAng Angle, GraphPoint p)
//{
//	double x, y;
//
//	x = -sin(Angle.a) * p.x + cos(Angle.a) * p.y;
//	y = -cos(Angle.a) * sin(Angle.b) * p.x - sin(Angle.a) * sin(Angle.b) * p.y + cos(Angle.b) * p.z;
//
//	putPixel(WIDTH / 2 + x, HEIGHT / 2 - y, RGB(p.x, p.y, p.z));
//}