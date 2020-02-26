//by 蔡曜，学号：3170105066
/*这个游戏名叫守卫堡垒，玩法就是尽可能躲开红色的导弹，存活时间越长越好。游戏中绿色方块为我方堡垒，黑色方块为敌人飞机，红色圆形为敌人导弹，会对堡垒造成一定伤害，蓝色圆形为补给品，堡垒吃下后可以发射蓝色炮弹，子弹可以消灭敌人和导弹。游戏有四种难度设定，分别是简单、中等、困难、末日，不同难度下导弹的初始速度、伤害、补给品的刷新时间都有不同。*/

#include"all.h"

int Setup(void)		//整个程序的入口
{
	initWindow("守卫堡垒", DEFAULT, DEFAULT, WIDTH, HEIGHT);	//生成游戏窗口
	
	registerTimerEvent(timerEvent);		//设定接收鼠标动作的函数，在control.c中
	registerMouseEvent(mouseEvent);		//设定定时器

	//int music[10];
	//loadSound("victory.mp3", &music[0]);
	//loadSound("Five Hours.mp3", &music[1]);
	//loadSound("穆特小提琴1.mp3", &music[2]);
	//playSound(music[0], 1);

	state = START;						//表示游戏为开始阶段

	startTimer(0, INTERVAL);			//开始定时器，INTERVAL记录刷新周期（单位：ms）

	return 0;
}

void timerEvent(int tid)				//定时触发的函数
{
	if (state == PROCESS) {				//当游戏为进行阶段
		gameTime += INTERVAL / 1000.0;	//gameTime计算一局游戏时间
		displayAll(&allObj);			//将所有游戏元素输出在屏幕上，allObj为记录所有元素的全局变量
		decideCrash(&allObj);			//判断是否有碰撞发生
		changeAll(&allObj);				//改变一次所有元素，例如使某个子弹往上移动2个像素
	}
	else if (state == START) {			//当游戏为开始阶段
		displayAll(&allObj);			
	}
	else if (state == END) {			//当游戏为结束阶段
		displayAll(&allObj);
	}
}