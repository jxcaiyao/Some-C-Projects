#include"All.h"

int Setup(void)
{
	initWindow("ÒýÁ¦Ä£Äâ", DEFAULT, DEFAULT, WIDTH, HEIGHT);
	//initConsole();

	registerMouseEvent(mouseEvent);
	registerTimerEvent(TimerEvent);

	initStars();
	initQStars(QStars, NUM);
	startTimer(0, INTERVAL);

	//initSwatch(Swatch);
	//Display(Angle, Swatch, 3 * 256 * 256 - 3 * 255 - 2);
	//Display(Angle, Stars, NUM);

	return 0;
}