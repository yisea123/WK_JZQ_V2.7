

#ifndef __ENTERNET_H
#define __ENTERNET_H


#ifdef  __cplusplus
	extern "C" {
#endif



//网络参数
extern u8 Sub_Mask[4];
extern u8 IP_Addr[4];
extern u8 Gateway_IP[4];
extern u8 DNS_SERVER[4];//DNS服务器IP地址
extern u8 NTP_SERVER[4];

//温控数据上传服务器地址
extern u8 SERVER_IP[4];
extern u16 SERVER_PORT;






typedef struct 
{
	u8 *myip;
	u8 *submask;
	u8 *getwayip;
	
}net_parameters;

//等待TCP发送完成
u8 net_wailt_sended(u8 socket);

//通过udp网络发送数据，需提供目的地ip地址和端口
u8 udp_send(SOCKET s,u8 *ip,u16 com,u8 *databuff,u16 size);

//udp初始化
u8 udp_init(SOCKET s,u16 mycom);

//通过tcp连接
u8 tcp_connect(SOCKET s,u16 mycom,u8 *ip,u16 com);

//通过tcp发送数据
u8 tcp_send(SOCKET s,u8 *databuff,u16 size);

//接收数据
u8 socket_recv(SOCKET s,u8 *databuff);

//关闭tcp连接
u8 tcp_close(SOCKET s);

//关闭udp连接
u8 udp_close(SOCKET s);

//设置网络参数
void net_set_getway(u8 *getwayip);
void net_set_sub_mask(u8 *mask);
void net_set_myip(u8 *myip);
void net_set_disip(u8 socket,u8 *disip,u16 discom,u16 mycom);

//网络参数初始化
void net_init(net_parameters *t);

//网络，设备启动时的初始化
void net_sysboot_init(void);

//获取物理连接状态
u8 net_get_phycstate(void);

//获取端口网络连接状态
u8 net_get_comstate(u8 socket);

//检查网关是否存在
u8 net_check_gateway(void);

//0没有初始化好，1初始化好了
u8 net_check_parameters(void);

//用于测试的，在这个函数内改变各项网络参数
void net_test(void);



#ifdef  __cplusplus
	}
#endif




#endif















