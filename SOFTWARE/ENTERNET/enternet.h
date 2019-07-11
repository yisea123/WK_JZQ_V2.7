

#ifndef __ENTERNET_H
#define __ENTERNET_H


#ifdef  __cplusplus
	extern "C" {
#endif


//����IP��ַ��ͬʱ������
#define SAME_IP(ip1,ip2) ((ip1[0]==ip2[0])&&(ip1[1]==ip2[1])&&(ip1[2]==ip2[2])&&(ip1[3]==ip2[3]))		
		
		

//�������
extern u8 Sub_Mask[4];
extern u8 IP_Addr[4];
extern u8 Gateway_IP[4];
extern u8 DNS_SERVER[4];//DNS������IP��ַ
extern u8 NTP_SERVER[4];

//�¿������ϴ���������ַ
extern u8 SERVER_IP[4];
extern u16 SERVER_PORT;






typedef struct 
{
	u8 *myip;
	u8 *submask;
	u8 *getwayip;
	
}net_parameters;


void set_ntp_serverip (u8 *ip);

void set_dns_serverip (u8 *ip);




//�ȴ�TCP�������
u8 net_wailt_sended(u8 socket);

//ͨ��udp���緢�����ݣ����ṩĿ�ĵ�ip��ַ�Ͷ˿�
u8 udp_send(SOCKET s,u8 *ip,u16 com,u8 *databuff,u16 size);

//udp��ʼ��
u8 udp_init(SOCKET s,u16 mycom);

//ͨ��tcp����
u8 tcp_connect(SOCKET s,u16 mycom,u8 *ip,u16 com);

//ͨ��tcp��������
u8 tcp_send(SOCKET s,u8 *databuff,u16 size);

//��������
u8 socket_recv(SOCKET s,u8 *databuff);

//�ر�tcp����
u8 tcp_close(SOCKET s);

//�ر�udp����
u8 udp_close(SOCKET s);

//�ж϶˿�״̬���ر�
u8 socket_close (SOCKET s);

//��ȡ�޷��ִ��IP��ַ���˿ں�
u8 net_get_unsenddir (u8 *ip,u16 *port);


//�����������
void net_set_getway(u8 *getwayip);
void net_set_sub_mask(u8 *mask);
void net_set_myip(u8 *myip);
void net_set_disip(u8 socket,u8 *disip,u16 discom,u16 mycom);

//���������ʼ��
void net_init(net_parameters *t);

//���磬�豸����ʱ�ĳ�ʼ��
void net_sysboot_init(void);

//��ȡ��������״̬
u8 net_get_phycstate(void);

//��ȡ�˿���������״̬
u8 net_get_comstate(u8 socket);

//��������Ƿ����
u8 net_check_gateway(void);

//0û�г�ʼ���ã�1��ʼ������
u8 net_check_parameters(void);

//���ڲ��Եģ�����������ڸı�����������
void net_test(void);




#ifdef  __cplusplus
	}
#endif




#endif















