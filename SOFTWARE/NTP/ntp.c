#include "includes.h"
#include "w5500.h"
#include "enternet.h"
#include "ntp.h"





u32 ntp_gettime (u8 socket,u8 *serverip)
{
	u8 *senddata=mymalloc (48);
	u32 time=0;
	ntpformat NTPformat;
	NTPformat.leap = 0;           /* leap indicator */ 
	NTPformat.version = 3;        /* version number */
	NTPformat.mode = 3;           /* mode */
	NTPformat.stratum = 0;        /* stratum */
	NTPformat.poll = 0;           /* poll interval */
	NTPformat.precision = 0;      /* precision */
	NTPformat.rootdelay = 0;      /* root delay */
	NTPformat.rootdisp = 0;       /* root dispersion */
	NTPformat.refid = 0;          /* reference ID */
	NTPformat.reftime = 0;        /* reference time */
	NTPformat.org = 0;            /* origin timestamp */
	NTPformat.rec = 0;            /* receive timestamp */
	NTPformat.xmt = 1;            /* transmit timestamp */
	
	mymemset(senddata,0,48);
	senddata[0]=(NTPformat.leap<<6)|(NTPformat.version<<3)|NTPformat.mode; 
	while (udp_init(socket,123)!=TRUE);
	udp_send(socket,serverip,123,senddata,48);
	myfree(senddata);
	
	u8 i=0;
	for( i=0;!checkSocketState(socket,IR_RECV);i++)
	{
		delay_ms(50);
		if (i>40) break;
	}
	if (i<40)
	{
		u8 *buff=mymalloc (2048);
		socket_recv(socket,buff);
		time=(buff[8+40]<<24)|(buff[8+41]<<16)|(buff[8+42]<<8)|(buff[8+43]);
		myfree(buff);
	}
	return time;
}




