
#ifndef __BEEP_H
#define __BEEP_H

#ifdef  __cplusplus
	extern "C" {
#endif



#define BEEP PCout(13)

typedef struct 
{
	char tone[4];//声调，1_低音，1，中音，1~，高音 0，间歇
	char time[4];//1/8，8分之一音，1/4，四分之一音
} jianpu;




//蜂鸣器引脚初始化
void BEEP_Init(void);

//设置音调的基础频率
void Beep_Set_Multiple (u32 multiple);

//把播放数组中的音调转换为频率并播放
void Beep_Play (jianpu *jianpu_);

//停止蜂鸣器播放
void Beep_End (void);

//在播放数组中添加一个音
void set_jianpu(jianpu *j,char *tone_,char *time_);

//在播放数组中添加结束符
void jianpu_end(jianpu *j);

//播放内置旋律
void Load_song(void);

//播放内置旋律1
void Load_song1(void);

//播放内置旋律2
void Load_song2(void);

//播放内置旋律3
void Load_song3(void);

//播放内置旋律up
void Load_up(void);

//播放内置旋律down
void Load_down(void);

//加载缓存里的旋律并播放
u8 beep_playBy (char *buff);

//开启蜂鸣器
void Beep_On (u16 _10ms);

//关闭蜂鸣器
void Beep_Off (void);


#ifdef  __cplusplus
	}
#endif





#endif



