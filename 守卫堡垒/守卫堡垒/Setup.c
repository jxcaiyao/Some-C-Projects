//by ���ף�ѧ�ţ�3170105066
/*�����Ϸ�����������ݣ��淨���Ǿ����ܶ㿪��ɫ�ĵ��������ʱ��Խ��Խ�á���Ϸ����ɫ����Ϊ�ҷ����ݣ���ɫ����Ϊ���˷ɻ�����ɫԲ��Ϊ���˵�������Ա������һ���˺�����ɫԲ��Ϊ����Ʒ�����ݳ��º���Է�����ɫ�ڵ����ӵ�����������˺͵�������Ϸ�������Ѷ��趨���ֱ��Ǽ򵥡��еȡ����ѡ�ĩ�գ���ͬ�Ѷ��µ����ĳ�ʼ�ٶȡ��˺�������Ʒ��ˢ��ʱ�䶼�в�ͬ��*/

#include"all.h"

int Setup(void)		//������������
{
	initWindow("��������", DEFAULT, DEFAULT, WIDTH, HEIGHT);	//������Ϸ����
	
	registerTimerEvent(timerEvent);		//�趨������궯���ĺ�������control.c��
	registerMouseEvent(mouseEvent);		//�趨��ʱ��

	//int music[10];
	//loadSound("victory.mp3", &music[0]);
	//loadSound("Five Hours.mp3", &music[1]);
	//loadSound("����С����1.mp3", &music[2]);
	//playSound(music[0], 1);

	state = START;						//��ʾ��ϷΪ��ʼ�׶�

	startTimer(0, INTERVAL);			//��ʼ��ʱ����INTERVAL��¼ˢ�����ڣ���λ��ms��

	return 0;
}

void timerEvent(int tid)				//��ʱ�����ĺ���
{
	if (state == PROCESS) {				//����ϷΪ���н׶�
		gameTime += INTERVAL / 1000.0;	//gameTime����һ����Ϸʱ��
		displayAll(&allObj);			//��������ϷԪ���������Ļ�ϣ�allObjΪ��¼����Ԫ�ص�ȫ�ֱ���
		decideCrash(&allObj);			//�ж��Ƿ�����ײ����
		changeAll(&allObj);				//�ı�һ������Ԫ�أ�����ʹĳ���ӵ������ƶ�2������
	}
	else if (state == START) {			//����ϷΪ��ʼ�׶�
		displayAll(&allObj);			
	}
	else if (state == END) {			//����ϷΪ�����׶�
		displayAll(&allObj);
	}
}