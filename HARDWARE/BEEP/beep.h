
#ifndef __BEEP_H
#define __BEEP_H

#ifdef  __cplusplus
	extern "C" {
#endif



#define BEEP PCout(13)

typedef struct 
{
	char tone[4];//������1_������1��������1~������ 0����Ъ
	char time[4];//1/8��8��֮һ����1/4���ķ�֮һ��
} jianpu;




//���������ų�ʼ��
void BEEP_Init(void);

//���������Ļ���Ƶ��
void Beep_Set_Multiple (u32 multiple);

//�Ѳ��������е�����ת��ΪƵ�ʲ�����
void Beep_Play (jianpu *jianpu_);

//ֹͣ����������
void Beep_End (void);

//�ڲ������������һ����
void set_jianpu(jianpu *j,char *tone_,char *time_);

//�ڲ�����������ӽ�����
void jianpu_end(jianpu *j);

//������������
void Load_song(void);

//������������1
void Load_song1(void);

//������������2
void Load_song2(void);

//������������3
void Load_song3(void);

//������������up
void Load_up(void);

//������������down
void Load_down(void);

//���ػ���������ɲ�����
u8 beep_playBy (char *buff);

//����������
void Beep_On (u16 _10ms);

//�رշ�����
void Beep_Off (void);


#ifdef  __cplusplus
	}
#endif





#endif



