

#ifndef SWD_H__
#define SWD_H__


#ifdef __cplusplus
	extern "C" {
#endif


//命令字定义											
#define SWD_REG_DP 0x00
#define SWD_REG_AP 0x02
#define SWD_READ	 0x04
#define SWD_WRITE	 0x00

//DP寄存器											
#define SWD_REG_IDCODE 			(0x00<<1)//只读
#define SWD_REG_ABORT 			(0x00<<1)//只写
#define SWD_REG_CTRL_STAT		(0x04<<1)//读写
#define SWD_REG_RESEND			(0x08<<1)//只读
#define SWD_REG_SELECT			(0x08<<1)//只写
#define SWD_REG_RDBUFF			(0x0c<<1)//只读

//AP寄存器
#define SWD_REG_CSW					(0x00<<1)
#define SWD_REG_TAR					(0x04<<1)										
#define SWD_REG_BASE				(0x08<<1)
#define SWD_REG_DRW					(0x0c<<1)
#define SWD_REG_IDR					(0x0c<<1)

//通信错误类型
#define SWD_ERR_OK					(0x01)
#define SWD_ERR_WAIT				(0x02)
#define SWD_ERR_FAIL				(0x04)
#define SWD_ERR_OTHER				(0x07)



// Cortex M3 Debug Registers (AHB addresses)
#define CM3_DDFSR   0xE000ED30      // Debug Fault StatusRegister
#define CM3_DHCSR   0xE000EDF0      // Debug Halting Control and Status Register
#define CM3_DCRSR   0xE000EDF4      // Debug Core Register Selector Register
#define CM3_DCRDR   0xE000EDF8      // Debug Core Register Data Register
#define CM3_DEMCR   0xE000EDFC      // Debug Exception and Monitor Control Register
#define CM3_AIRCR   0xE000ED0C      // The Application Interrupt and Reset Control Register

//读写内核寄存器
#define CM3_REG_READ 		0x00000000
#define CM3_REG_WRITE		0x00010000

//内核寄存器选址
#define CM3_REG_R0      0x0000
#define CM3_REG_R1      0x0001
#define CM3_REG_R2      0x0002
#define CM3_REG_R3      0x0003
#define CM3_REG_R4      0x0004
#define CM3_REG_R5      0x0005
#define CM3_REG_R6      0x0006
#define CM3_REG_R7      0x0007
#define CM3_REG_R8      0x0008
#define CM3_REG_R9      0x0009
#define CM3_REG_R10     0x000a
#define CM3_REG_R11     0x000b
#define CM3_REG_R12     0x000c
#define CM3_REG_R13     0x000d
#define CM3_REG_SP      0x000d
#define CM3_REG_R14     0x000e
#define CM3_REG_LR      0x000e
#define CM3_REG_R15     0x000f
#define CM3_REG_PC      0x000f
#define CM3_REG_xPSR    0x0010
#define CM3_REG_MSP     0x0011
#define CM3_REG_PSP     0x0012



//初始化
void SWD_Init (void);

//总线复位
void SWD_LineReset (void);

//读取寄存器
u32 SWD_ReadReg (u8 cmd,u8 *err);

//写入寄存器
void SWD_WriteReg (u8 cmd,u8 *err,u32 data);

//向内存写入数据,成功返回设备id
u32 SWD_WriteSram (u32 addr,u32 *data,u32 len);

//向内存读取数据,成功返回设备id
u32 SWD_ReadSram (u32 addr,u32 *data,u32 len);


//暂停或者启动内核1，暂停，0，启动
u32 SWD_Cm3Halt (u8 value);

//向内核寄存器写入数据
u32 SWD_WriteCm3Reg (u16 reg_select,u32 data);

#ifdef __cplusplus
	}
#endif
		




#endif




