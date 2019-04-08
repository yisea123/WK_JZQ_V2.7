#include "sys.h"
#include "my_messeg.h"



	//把错误转换为字符串
char *err_to_str(u16 err)
{
	switch (err)
	{
		case ERR_SUCCESS:
			return "成功";
		case ERR_FAIL:
			return "未知错误";
		case ERR_DATAVALE:
			return "数据错误";
		case ERR_DATALENGTH:
			return "数据长度错误";
		case ERR_WANTLENGTH:
			return "想要读取的数据长度错误";
		case ERR_NULLCMD:
			return "不支持的命令类型";
		case ERR_NOCONNECT:
			return "没有连接";
		case ERR_NOCJADDR:
			return "采集器的地址不是本机绑定的采集器地址";
		case ERR_CANNOTCFG:
			return "在不是可配置状态下收到了配置命令";
		case ERR_NOTMYTYPE:
			return "所要控制的设备与本机不符";
		case ERR_TIMEOUT:
			return "超时";
		case ERR_OFFLINE:
			return "设备不在线";
		case ERR_NONEADDR:
			return "设备地址不对";
		case ERR_ADDRCROSS:
			return "添加设备时地址越界";
		default:
			return "其他错误";
	}
}




