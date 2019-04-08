#ifndef __MY_RF_H
#define __MY_RF_H


/****************************************************************

		EN_CONFIG最后20位存储IP地址相关数据



*****************************************************************/







#ifdef __cplusplus
	extern "C" {
#endif








//处理消息循环
void my_rf_loop (void * t);

//无线的数据处理
void my_rf_deal (void * t);

//处理手动消息
void my_rf_hand (void * t);



//获取设备信息，recv是接收数组
u16 Cmd_0x01 (u16 addr ,u8 *recv);

//控制设备
u16 Cmd_0x03 (u16 addr ,u8 type,u8 power,u8 state);

//进入退出配置模式
u16 Cmd_0x06 (u16 addr ,u8 state);

//配置基本参数
u16 Cmd_0x07 (u16 addr ,u8 type);








void loushui_warn(u16 addr,u8 devtype);

void rf_cjq_deal(u8 *data);


u16 CheckId (u16 addr);//判断是否含有此设备id


					//设备运行状态指示，类型，开关量，状态（升降）
void Device_state_see(u8 type,u8 power,u8 state);

						//获取新的控制设备的状态
u16 get_newstate(u8 nowpower,u8 key,u16 addr);
							//获取一体机的新状态
u16 yt_newstate(u8 nowpower,u8 key,u16 addr);

									//更新集中器中记录的设备状态
u16 Updata_devicestate(u16 addr ,u8 power,u8 state);





#ifdef __cplusplus
	}
#endif





#endif


