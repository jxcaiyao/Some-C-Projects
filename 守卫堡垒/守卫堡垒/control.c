#include"all.h"

void mouseEvent(int x, int y, int bt, int event)		//接收鼠标操作的函数
{
	if (state == START) {
		if (bt == LEFT_BUTTON) {	//选择游戏难度
			if (x > WIDTH / 2 - 34 && y > HEIGHT / 2 - 59.5 && x < WIDTH / 2 + 34 && y < HEIGHT / 2 - 25.5) {
				setDifficulty(EASY);
			}
			else if (x > WIDTH / 2 - 34 && y > HEIGHT / 2 - 17 && x < WIDTH / 2 + 34 && y < HEIGHT / 2 + 17) {
				setDifficulty(MEDIUM);
			}
			else if (x > WIDTH / 2 - 34 && y > HEIGHT / 2 + 25.5 && x < WIDTH / 2 + 34 && y < HEIGHT / 2 + 59.5) {
				setDifficulty(HARD);
			}
			else if (x > WIDTH / 2 - 34 && y > HEIGHT / 2 + 68 && x < WIDTH / 2 + 34 && y < HEIGHT / 2 + 102) {
				setDifficulty(DOOM);
			}
		}
	}
	else if (state == PROCESS && x > 0 && y > 0 && x < WIDTH && y < HEIGHT) {	//堡垒跟随鼠标
		allObj.bat->p.x = x;
		allObj.bat->p.y = y;
	}
	else if (state == END) {
		if (bt == LEFT_BUTTON) {		//选择重新开始
			if (x >= 320 && y >= 400 && x <= 480 && y <= 441) {
				freePlane(allObj.pll);	//释放游戏元素所占内存
				freeBomb(allObj.bol);
				freeBullet(allObj.bul);
				freeCrash(allObj.crl);
				freeAward(allObj.awl);

				state = START;
			}
		}
	}
}
