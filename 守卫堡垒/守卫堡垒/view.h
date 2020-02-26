#include"model.h"

#ifndef __VIEW__
#define __VIEW__

#define WIDTH 800		//窗口宽度
#define HEIGHT 600		//窗口高度

void displayAll(allObject *allObj);
void displayBackground(void);
void displayPlane(planeList *pList);
void planeImg(Plane *pl);
void displayBomb(bombList *bList);
void bombImg(Bomb *bo);
void displayBattery(Battery *ba);
void batteryImg(Battery *ba);
void displayCrash(crashList *cList);
void crashImg(Crash *cr);
void displayBullet(bulletList *bList);
void bulletImg(Bullet *bu);
void displayAward(awardList *aList);
void AwardImg(Award *aw);

void displayFrame(void);
void displayTitle(char *str);
void displaySelection(void);
void displayTime(double gameTime);
char* DtoS(double gameTime);
void displayRecord(void);
void displayRestart(void);

#endif