
#ifndef NTP_H__
#define NTP_H__

#ifdef __cplusplus
	extern "C" {
#endif

typedef signed char s_char;
typedef unsigned long long tstamp;
typedef unsigned int tdist;
		
		
		
typedef struct _ntpformat
{
   
		u8  dstaddr[4];        /* destination (local) address */
		char    version;        /* version number */
		char    leap;           /* leap indicator */
		char    mode;           /* mode */
		char    stratum;        /* stratum */
		char    poll;           /* poll interval */
		s_char  precision;      /* precision */
		tdist   rootdelay;      /* root delay */
		tdist   rootdisp;       /* root dispersion */
		char    refid;          /* reference ID */
		tstamp  reftime;        /* reference time */
		tstamp  org;            /* origin timestamp */
		tstamp  rec;            /* receive timestamp */
		tstamp  xmt;            /* transmit timestamp */
      

} ntpformat;
		
		
		
u32 ntp_gettime (u8 socket,u8 *serverip);
		
		
		
		
		

#ifdef __cplusplus
	}
#endif


#endif


