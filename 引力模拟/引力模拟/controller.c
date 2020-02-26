#include"All.h"

void mouseEvent(int x1, int y1, int bt, int event)
{
	static double x0, y0, dx, dy;
	static int flag = 0;

	//if (x1 == WIDTH/2 && y1 == HEIGHT/2) {
	//	return;
	//}
	//printf("x1 = %d, y1 = %d, bt = %d, event = %d, flag = %d\n", x1, y1, bt, event, flag);
	//printf("ag0 = %f, ag1 = %f, dag = %f\n", ag0, ag1, dag);

	if (bt == MIDDLE_BUTTON) {
		return;
	}

	//printf("x0 = %f, y0 = %f, x1 = %f, y1 = %f\n", x0, y0, x1, y1);

	if (flag == 0 && bt == LEFT_BUTTON && event == 0) {
		flag = 1;
	}
	else if (flag == 1 && bt == LEFT_BUTTON && event == 2) {
		flag = 0;
	}

	if (flag == 0) {
		x0 = x1;
		y0 = y1;
		return;
	}
	else if(flag == 1){
		dx = x1 - x0;
		dy = y1 - y0;

		Angle.a = fmod(Angle.a - dx / 600 * PI, 2*PI);
		Angle.b = fmod(Angle.b + dy / 600 * PI, 2*PI);

		//printf("a = %f, b = %f\n", Angle.a, Angle.b);

		//Display(Angle, Swatch, 3 * 256 * 256 - 3 * 255 - 2);
		//Display(Angle, Stars, 2);

		x0 = x1;
		y0 = y1;
	}
}