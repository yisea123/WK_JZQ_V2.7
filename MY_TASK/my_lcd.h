#ifndef __MY_LCD_H
#define __MY_LCD_H



#ifdef __cplusplus
	extern "C" {
#endif







#define  PAGE_MAIN    0x01		//��ҳ
#define  PAGE_GSET    0x02		//�߼�����
#define  PAGE_PASS    0x03		//����


//LCD��Ļ����
void my_lcd (void *);

		
		
/*******************��Ļ��������*****************/		
		
//��Ļ��ʼ��
void my_lcd_init (void);

//����һ��ҳ��
void lcd_page (u8 page);

//�ı�һ�����ص�״̬
void lcd_chan_state (u8 type,u8 state);

void lcd_set_data (u8 type,u16 data);

//�����ȱ���
void lcd_beep(u8 l);

//��Ļ�ϴ���ʪ������
void lcd_dataUp (u8 *);

//����˯��
void LCD_tosleep (void);

//��������
void LCD_tolight (void);

//��ȡ��Ļ��ʱ��
u8 lcd_gettime (u16 *hour,u16 *min,u16 *sec);

/*******************��Ļ��������End*****************/		





/*********��Ļ����������***************************/
void Set_Init(void);//���ó�ʼ��

void Hand_Set_Init(void);//�ֶ����ó�ʼ������

void Set_Save(void);//����ҳ�汣��

void Auto_sleep(void);//����ʱ���

void Time_Set(void);//����ʱ�����ñ���

//��Ļ˯�߰��������Լ�����Ϣ˯��
void lcd_sleep(void);

void Into_Senior(u8 *buf);//����߼�����

void BUT_UP_TEMP(void);//�ֶ��������°���
void BUT_DOWN_TEMP(void);//�ֶ����ý��°���
void BUT_UP_HUMI(void);
void BUT_DOWN_HUMI(void);
void BUT_PM(void);
void AutoCtr(void);//�ֶ��Զ�ת��
/*********��Ļ����������End***************************/




//��Ļ��Ϣ����02
void  lcd_messeg_02 (u8 *meg);//��Ļ���ݴ���
void	lcd_messeg_03 (u8 *meg);//��Ļ��Դ����






				



void lcd_timetosleep(void);//�Զ�����������Լ5���ж�һ��






#ifdef __cplusplus
	}
#endif




#endif


