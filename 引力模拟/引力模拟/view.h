#pragma once

#define WIDTH 1200
#define HEIGHT 700

void Display(VisAng Angle, CelBdy Stars[], int num);
void DisplayBody(VisAng Angle, CelBdy Star);
void DisplayLine(VisAng Angle, Vector p1, Vector p2);
void DisplayOrbit(VisAng Angle, Queue PtrQ, int index);
//void DisplayPixel(VisAng Angle, GraphPoint p);