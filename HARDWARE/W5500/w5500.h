#ifndef	_W5500_H_
#define	_W5500_H_



#ifdef  __cplusplus
	extern "C" {
#endif






#define SOCKET u8
#define MAC ((u8*)0x1FFFF7E8)

/***************** Common Register *****************/
#define MR		0x0000					//模式寄存器
	#define RST		0x80					//该位为1时内部寄存器初始化
	#define WOL		0x20					//1，开启网络唤醒，0，关闭网络唤醒
	#define PB		0x10					//1，不理会其他主机的PING，0，理会
	#define PPP		0x08					//1，启用PPPoE模式，0，不使用PPPoE
	#define FARP	0x02					//1，启用强迫ARP模式，0，不启用

#define GAR		0x0001					//网关IP地址寄存器
#define SUBR	0x0005					//子网掩码寄存器
#define SHAR	0x0009					//本机MAC地址寄存器
#define SIPR	0x000f					//本机IP地址寄存器

#define INTLEVEL	0x0013			//中断延时寄存器
#define Sn_PROTO  0x0014			//
	#define IPPROTO_IP                   0        /**< Dummy for IP */
	#define IPPROTO_ICMP                 1        /**< Control message protocol */
	#define IPPROTO_IGMP                 2        /**< Internet group management protocol */
	#define IPPROTO_GGP                  3        /**< Gateway^2 (deprecated) */
	#define IPPROTO_TCP                  6        /**< TCP */
	#define IPPROTO_PUP                  12       /**< PUP */
	#define IPPROTO_UDP                  17       /**< UDP */
	#define IPPROTO_IDP                  22       /**< XNS idp */
	#define IPPROTO_ND                   77       /**< UNOFFICIAL net disk protocol */
	#define IPPROTO_RAW                  255      /**< Raw IP packet */

#define IR				0x0015				//中断寄存器地址
	#define CONFLICT	0x80				//APR请求时发现对方的IP地址与本机相同
	#define UNREACH		0x40				//目标地址不可达
	#define PPPOE			0x20				//PPPoE连接关闭
	#define MP				0x10				//网络唤醒功能开启时，收到了网络唤醒包

#define IMR				0x0016				//中断屏蔽寄存器地址
	#define IM_IR7		0x80				//以下位值1时启用对应中断
	#define IM_IR6		0x40
	#define IM_IR5		0x20
	#define IM_IR4		0x10

#define SIR				0x0017				//SOCKET中断寄存器
	#define S7_INT		0x80				//以下位值1时表示对应SOCKET发生了中断
	#define S6_INT		0x40
	#define S5_INT		0x20
	#define S4_INT		0x10
	#define S3_INT		0x08
	#define S2_INT		0x04
	#define S1_INT		0x02
	#define S0_INT		0x01

#define SIMR			0x0018				//SOCKET中断屏蔽寄存器
	#define S7_IMR		0x80				//以下位值1时启用对应端口的中断
	#define S6_IMR		0x40
	#define S5_IMR		0x20
	#define S4_IMR		0x10
	#define S3_IMR		0x08
	#define S2_IMR		0x04
	#define S1_IMR		0x02
	#define S0_IMR		0x01

#define RTR		0x0019						//重试时间值寄存器，单位为100微妙
#define RCR		0x001b						//重试计数寄存器，重试RCR+1次时发生超时中断

#define PTIMER	0x001c
#define PMAGIC	0x001d
#define PHA		0x001e
#define PSID	0x0024
#define PMRU	0x0026

#define UIPR	0x0028						//无法抵达IP地址寄存器
#define UPORT	0x002c						//无法抵达端口号寄存器

#define PHYCFGR	0x002e					//配置寄存器
	#define RST_PHY		0x80				//该为值0时重启内部PHY
	#define OPMODE		0x40				//
	#define DPX			0x04					//1，全双工，0，半双工
	#define SPD			0x02					//1，百兆，0，十兆
	#define LINK		0x01					//1，有网线连接，0，无网线连接

#define VERR	0x0039

/********************* Socket Register *******************/
#define Sn_MR		0x0000
	#define MULTI_MFEN		0x80
	#define BCASTB			0x40
	#define	ND_MC_MMB		0x20
	#define UCASTB_MIP6B	0x10
	#define MR_CLOSE		0x00			//本端口使用的协议模式
	#define MR_TCP		0x01
	#define MR_UDP		0x02
	#define MR_MACRAW		0x04			//只有端口0支持此模式
	#define Sn_MR_IPRAW   0x03      /**< IP LAYER RAW SOCK */

#define Sn_CR		0x0001					//端口配置寄存器
	#define SN_OPEN		0x01				//按MR设置的通信协议打开SOCKET
	#define SN_LISTEN		0x02			//在MR是TCP时配置为服务器
	#define SN_CONNECT		0x04		//在MR是TCP时配置为客户端
	#define SN_DISCON		0x08			//在MR是TCP时断开连接
	#define SN_CLOSE		0x10			//关闭SOCKET
	#define SN_SEND		0x20				//发送内存中的数据
	#define SN_SEND_MAC	0x21			//UDP模式时通过指定MAC地址发送数据
	#define SN_SEND_KEEP	0x22		//在MR是TCP时发送KEEP心跳包
	#define SN_RECV		0x40				//

#define Sn_IR		0x0002					//端口中断寄存器
	#define IR_SEND_OK		0x10		//发送完成
	#define IR_TIMEOUT		0x08		//ARP或TCP超时
	#define IR_RECV			0x04			//有数据接收
	#define IR_DISCON		0x02			//收到对方的断开命令
	#define IR_CON			0x01			//TCP成功连接

#define Sn_SR		0x0003					//状态寄存器
	#define SOCK_CLOSED		0x00		//端口关闭
	#define SOCK_INIT		0x13			//在TCP协议OPEN成功
	#define SOCK_LISTEN		0x14		//工作在TCP服务器模式下等待客户端连接
	#define SOCK_ESTABLISHED	0x17//TCP在连接状态下
	#define SOCK_CLOSE_WAIT		0x1c//TCP半关闭状态
	#define SOCK_UDP		0x22			//UDP协议OPEN成功
	#define SOCK_MACRAW		0x02		//MACRAW模式OPEN成功
	#define SOCK_IPRAW        0x32     /**< ip raw mode socket */

	#define SOCK_SYNSEND	0x15		//TCP连接时发送了连接请求
	#define SOCK_SYNRECV	0x16		//TCP连接时接收了连接请求
	#define SOCK_FIN_WAI	0x18		//正在关闭
	#define SOCK_CLOSING	0x1a		//正在关闭
	#define SOCK_TIME_WAIT	0x1b	//正在关闭
	#define SOCK_LAST_ACK	0x1d		//正在等待断开请求回应

#define Sn_PORT		0x0004				//源端口寄存器
#define Sn_DHAR	   	0x0006			//目的MAC寄存器
#define Sn_DIPR		0x000c				//目标IP寄存器
#define Sn_DPORTR	0x0010				//目标端口寄存器

#define Sn_MSSR		0x0012				//最大分段寄存器
#define Sn_TOS		0x0015				//服务类型寄存器
#define Sn_TTL		0x0016				//IP头生存时间寄存器

#define Sn_RXBUF_SIZE	0x001e		//接收缓存大小寄存器
#define Sn_TXBUF_SIZE	0x001f		//发送缓存大小寄存机
#define Sn_TX_FSR	0x0020				//发送剩余缓存寄存器
#define Sn_TX_RD	0x0022				//发送读指针寄存器，数据发送完成之后此值会等于Sn_TX_WR
#define Sn_TX_WR	0x0024				//发送写指针寄存器，
#define Sn_RX_RSR	0x0026				//剩余接收缓存寄存器
#define Sn_RX_RSR1	0x0027
#define Sn_RX_RD	0x0028				//接收读指针寄存器
#define Sn_RX_WR	0x002a				//接收写指针寄存器

#define Sn_IMR		0x002c				//端口中断屏蔽寄存器
	#define IMR_SENDOK	0x10
	#define IMR_TIMEOUT	0x08
	#define IMR_RECV	0x04
	#define IMR_DISCON	0x02
	#define IMR_CON		0x01

#define Sn_FRAG		0x002d				//分段寄存器，它设置了 IP 层中 IP 报头的分段字段
#define Sn_KPALVTR	0x002f			//TCP心跳包验证时间，单位5秒，在本寄存器不为0的情况下会自动发送心跳包

/*******************************************************************/
/************************ SPI Control Byte *************************/
/*******************************************************************/
/* Operation mode bits */
#define VDM		0x00
#define FDM1	0x01
#define	FDM2	0x02
#define FDM4	0x03

/* Read_Write control bit */
#define RWB_READ	0x00
#define RWB_WRITE	0x04

/* Block select bits */
#define COMMON_R	0x00

/* Socket 0 */
#define S0_REG		0x08
#define S0_TX_BUF	0x10
#define S0_RX_BUF	0x18

/* Socket 1 */
#define S1_REG		0x28
#define S1_TX_BUF	0x30
#define S1_RX_BUF	0x38

/* Socket 2 */
#define S2_REG		0x48
#define S2_TX_BUF	0x50
#define S2_RX_BUF	0x58

/* Socket 3 */
#define S3_REG		0x68
#define S3_TX_BUF	0x70
#define S3_RX_BUF	0x78

/* Socket 4 */
#define S4_REG		0x88
#define S4_TX_BUF	0x90
#define S4_RX_BUF	0x98

/* Socket 5 */
#define S5_REG		0xa8
#define S5_TX_BUF	0xb0
#define S5_RX_BUF	0xb8

/* Socket 6 */
#define S6_REG		0xc8
#define S6_TX_BUF	0xd0
#define S6_RX_BUF	0xd8

/* Socket 7 */
#define S7_REG		0xe8
#define S7_TX_BUF	0xf0
#define S7_RX_BUF	0xf8

#define TRUE	0xff
#define FALSE	0x00

#define S_RX_SIZE	2048	/*定义Socket接收缓冲区的大小，可以根据W5500_RMSR的设置修改 */
#define S_TX_SIZE	2048  	/*定义Socket发送缓冲区的大小，可以根据W5500_TMSR的设置修改 */

/***************----- W5500 GPIO定义 -----***************/
#define W5500_SCS		GPIO_Pin_4	//定义W5500的CS引脚	 
#define W5500_SCS_PORT	GPIOA
	
#define W5500_RST		GPIO_Pin_5	//定义W5500的RST引脚
#define W5500_RST_PORT	GPIOC

#define W5500_INT		GPIO_Pin_4	//定义W5500的INT引脚
#define W5500_INT_PORT	GPIOC
















/***************----- 网络参数变量定义 -----***************/
extern u8 Phy_Addr[6];//硬件地址


extern u8 S0_State;
extern u8 S1_State;

extern u8 S0_Data;
extern u8 S1_Data;




/***************----- 端口的运行模式 -----***************/
#define TCP_SERVER		0x00	//TCP服务器模式
#define TCP_CLIENT		0x01	//TCP客户端模式 
#define UDP_MODE			0x02	//UDP(广播)模式 

/***************----- 端口的运行状态 -----***************/
#define S_INIT			0x01	//端口完成初始化 
#define S_CONN			0x02	//端口完成连接,可以正常传输数据 

/***************----- 端口收发数据的状态 -----***************/
#define S_RECEIVE			0x01		//端口接收到一个数据包 
#define S_TRANSMITOK	0x02		//端口发送一个数据包完成 

/***************----- 端口数据缓冲区 -----***************/

extern unsigned char W5500_Interrupt;	//W5500中断标志(0:无中断,1:有中断)
typedef unsigned char SOCKET;			//自定义端口号数据类型

extern void Delay(unsigned int d);//延时函数(ms)
extern void W5500_GPIO_Configuration(void);//W5500 GPIO初始化配置
extern void W5500_NVIC_Configuration(void);//W5500 接收引脚中断优先级设置
extern void SPI_Configuration(void);//W5500 SPI初始化配置(STM32 SPI1)
extern void W5500_Hardware_Reset(void);//硬件复位W5500
extern void W5500_Init(void);//初始化W5500寄存器函数
extern unsigned char Detect_Gateway(void);//检查网关服务器
extern void Socket_Init(SOCKET s);//指定Socket(0~7)初始化
extern unsigned char Socket_Connect(SOCKET s);//设置指定Socket(0~7)为客户端与远程服务器连接
extern unsigned char Socket_Listen(SOCKET s);//设置指定Socket(0~7)作为服务器等待远程主机的连接
extern unsigned char Socket_UDP(SOCKET s);//设置指定Socket(0~7)为UDP模式
extern unsigned short Read_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr);//指定Socket(0~7)接收数据处理
extern void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size); //指定Socket(0~7)发送数据处理
extern void W5500_Interrupt_Process(void);//W5500中断处理程序框架

void Write_W5500_1Byte(unsigned short reg, unsigned char dat);
unsigned char Read_W5500_1Byte(unsigned short reg);




void Delay(unsigned int d);			//延时函数(ms)

 
void W5500_Initialization(void);//系统调用这个函数初始化网口
void W5500_Socket_Set(void);



					//装载网络参数
void Load_Net_Parameters(void);




extern void	Write_W5500_SOCK_1Byte(SOCKET s,unsigned short reg,unsigned char dat);
extern void Write_W5500_SOCK_2Byte(SOCKET s, unsigned short reg, unsigned short dat);
extern void Write_W5500_SOCK_4Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr);
extern void Write_W5500_nByte(unsigned short reg, unsigned char *dat_ptr, unsigned short size);
extern void Read_W5500_nByte(unsigned short reg, unsigned char *dat_ptr, unsigned short size);
extern u8 Read_W5500_SOCK_1Byte(SOCKET s, unsigned short reg);
extern u8 Read_W5500_1Byte(unsigned short reg);





void W5500_IRQ (void);




//获取指定端口的状态
u8 getSocketState (u8 socket);

//清除指定端口的状态
void delSocketState (u8 socket,u8 states);





/*查询是否有指定状态，有则返回1,并清除，无返回0
	state取值范围可以是：
		IR_SEND_OK		消息发送成功
		IR_TIMEOUT		操作超时
		IR_RECV				有数据接收
		IR_DISCON			连接断开
		IR_CON				连接成功
************************************************/
u8 checkSocketState (u8 socket,u8 state);

//查询网络是否有指定状态，有则返回1,，无返回0
u8 checkSocketStateN (u8 socket,u8 state);

//查询网络状况
u8 checkNetstate (u8 state);

//查询网络状况,不清除
u8 checkNetstateN (u8 state);



#ifdef  __cplusplus
	}
#endif





#endif

