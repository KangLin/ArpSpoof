#ifndef MY_ARP_CHEAT_INCLUDE_H 
#define MY_ARP_CHEAT_INCLUDE_H 

#include <pcap.h> 
#include <remote-ext.h>
#include <conio.h> 
#include <packet32.h> 
#include <ntddndis.h>
#include <string>

#define ETH_IP              0x0800
#define ETH_ARP             0x0806
#define ARP_REQUEST         0x0001
#define ARP_REPLY           0x0002
#define ARP_HARDWARE        0x0001
#define MAC_LENGTH          0x0006
#define PROTOCOL_IP_LENGTH  0x0004

//�ֽڶ��������1 
#pragma pack (1) 
typedef struct ethernet_head 
{ 
	unsigned char dest_mac[6]; //Ŀ������MAC��ַ 
	unsigned char source_mac[6]; //Դ��MAC��ַ 
	unsigned short eh_type; //��̫������ 
} ETHERNET_HEAD; 

struct arp_head 
{ 
	unsigned short hardware_type; //Ӳ�����ͣ���̫���ӿ�����Ϊ1 
	unsigned short protocol_type; //Э�����ͣ�IPЭ������Ϊ0X0800 
	unsigned char add_len; //Ӳ����ַ���ȣ�MAC��ַ����Ϊ6B 
	unsigned char pro_len; //Э���ַ���ȣ�IP��ַ����Ϊ4B 
	unsigned short option; //������ARP����Ϊ1��ARPӦ��Ϊ2 
	unsigned char sour_addr[6]; //ԴMAC��ַ�����ͷ���MAC��ַ 
	unsigned long sour_ip; //ԴIP��ַ�����ͷ���IP��ַ 
	unsigned char dest_addr[6]; //Ŀ��MAC��ַ��ARP�����и��ֶ�û�����壻ARP��Ӧ��Ϊ���շ���MAC��ַ 
	unsigned long dest_ip; //Ŀ��IP��ַ��ARP������Ϊ���������IP��ַ��ARP��Ӧ��Ϊ���շ���IP��ַ
	unsigned char padding[18]; 
}; 

struct arp_packet //����arp���ṹ 
{ 
	ethernet_head eth; //��̫��ͷ�� 
	arp_head arp; //arp���ݰ�ͷ�� 
}; 
#pragma pack()

//�����̲߳����ṹ��
struct _PARA
{
	std::string devName;
	std::string fileName;
	std::string filter;
};

/*!
\brief   iptos
\param   u_long in��
\return  ����Ϊ char * ��
\version 1.0
\author  ��  ��
\date    2013/4/5 10:03:21
*/
char * iptos(u_long in);
/*!
\brief   ip6��ַת�����ַ���
\param   struct sockaddr * sockaddr��
\param   char * address            ��
\param   int addrlen               ��
\return  ����Ϊ char * ��
\version 1.0
\author  ��  ��
\date    2013/4/5 10:02:57
*/
char * ip6tos(struct sockaddr * sockaddr, char * address, int addrlen);
/*!
\brief   ��ӡ�ӿ���Ϣ
\param   pcap_if_t * d��
\return  ����Ϊ void ��
\version 1.0
\author  ��  ��
\date    2013/4/5 10:02:39
*/
int ifprint(pcap_if_t * d, void *para);

typedef int (*IfPrintCallBack)(pcap_if_t*d, void *para);

/*!
\brief   ��ʾ�ӿ���Ϣ
\param   IfPrintCallBack callBack:�ص�����
         void * para�������ص������Ĳ���ָ��
\return  ����Ϊ int ��
\version 1.0
\author  ��  ��
\date    2013/4/5 10:02:08
*/
int ListInterfaceInfomation(IfPrintCallBack callBack = NULL, void * para = NULL);

/** 
* ���������MAC��ַ 
* pDevName �������豸���� 
*/ 
unsigned char* GetSelfMac(char* pDevName, unsigned char* mac); 

/*!
\brief   ����arp��
\param   struct arp_packet * pPacket����ָ��
\param   unsigned char * pSrcMac    ��ԴMAC��ַ��6�ֽ�MAC��ַ����
\param   unsigned char * pDstMac    ��Ŀ��MAC��ַ��6�ֽ�MAC��ַ����
\param   unsigned long lSrcIp       ��ԴIP��4�ֽ�IP��ַ
\param   unsigned long lDstIp       ��Ŀ��IP
\return  ����Ϊ int ��
\version 1.0
\author  ��  ��
\date    2013/4/7 13:14:06
*/
int BuildArpPacket(
    /*[in/out]*/struct arp_packet * pPacket,
	/*[in]*/unsigned char * pSrcMac,
	/*[in]*/unsigned char * pDstMac,
	/*[in]*/unsigned long lSrcIp,
	/*[in]*/unsigned long lDstIp,
	/*[in]*/unsigned short option = ARP_REQUEST/*��������*/
	);

/*!
\brief   ����arp��
\param   struct arp_packet * pPacket��
\param   char * pSrcMac             ��ԴMAC��ַ���ַ��������磺��1234567890ab��
\param   char * pDstMac             ��Ŀ��MAC��ַ
\param   char * pSrcIp              ��ԴIP���ַ��������磺��192.168.1.111��
\param   char * pDstIp              ��Ŀ��IP
\return  ����Ϊ int ��
\version 1.0
\author  ��  ��
\date    2013/4/7 14:00:39
*/
int BuildArpPacket(
    /*[in/out]*/struct arp_packet * pPacket,
    /*[in]*/char * pSrcMac,
	/*[in]*/char * pDstMac,
	/*[in]*/char * pSrcIp,
	/*[in]*/char * pDstIp,
	/*[in]*/unsigned short option = ARP_REQUEST/*��������*/
	);
	
int BuildArpRequest(
					/*[in/out]*/struct arp_packet * pPacket,
					/*[in]*/char * pSrcMac,
					/*[in]*/char * pDstMac,
					/*[in]*/char * pSrcIp,
					/*[in]*/char * pDstIp
					);

int ArpSpoof(
		pcap_t * Handler, char * pszGatewayIp, char * pszGatewayMac,
		char * pszHostIp, char * pszHostMac, char * pszLocalMac
		);

int ArpSpoof(
			 char* pszInterfaceName, char * pszGatewayIp, char * pszGatewayMac,
			 char * pszHostIp, char * pszHostMac, char * pszLocalMac,
			 bool bGateway = true, bool bHost = true
			 );

int ArpSpoof(
			 char * pszInterfaceName, char * pszGatewayIp,
			 char * pszHostIp, int nInterval /*ms*/
			 );

/*!
\brief   ����IP�õ���Ӧ��MAC��ַ
\param   char * pszIp       ��ָ����IP���������
\param   unsigned char * Mac��MAC��ַ���������
\return  ����Ϊ int ��
\version 1.0.0.1
\author  ��  ��
\date    2013-4-10 8:33:53
*/
int GetMac( /*[in]*/char * pszIp, /*[out]*/unsigned char * Mac);

/*!
\brief   ��MAC����ת�����ַ��������̰߳�ȫ
\param   unsigned char * pMac��MAC��ַ����
\return  ����Ϊ const char* ��ת�����MAC��ַ�ַ�����
\version 1.0.0.1
\author  ��  ��
\date    2013-4-10 8:37:37
*/
const char* GetMacString(unsigned char * pMac);

int HexStringToCharArrary(char * pHexString, unsigned char* pCharArrary);
#endif 