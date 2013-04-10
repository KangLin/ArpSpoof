#ifndef MY_ARP_CHEAT_INCLUDE_H 
#define MY_ARP_CHEAT_INCLUDE_H 

#include <pcap.h> 
#include <remote-ext.h>
#include <conio.h> 
#include <packet32.h> 
#include <ntddndis.h>

#define ETH_IP              0x0800
#define ETH_ARP             0x0806
#define ARP_REQUEST         0x0001
#define ARP_REPLY           0x0002
#define ARP_HARDWARE        0x0001
#define MAC_LENGTH          0x0006
#define PROTOCOL_IP_LENGTH  0x0004

//字节对齐必须是1 
#pragma pack (1) 
struct ethernet_head 
{ 
	unsigned char dest_mac[6]; //目标主机MAC地址 
	unsigned char source_mac[6]; //源端MAC地址 
	unsigned short eh_type; //以太网类型 
}; 

struct arp_head 
{ 
	unsigned short hardware_type; //硬件类型：以太网接口类型为1 
	unsigned short protocol_type; //协议类型：IP协议类型为0X0800 
	unsigned char add_len; //硬件地址长度：MAC地址长度为6B 
	unsigned char pro_len; //协议地址长度：IP地址长度为4B 
	unsigned short option; //操作：ARP请求为1，ARP应答为2 
	unsigned char sour_addr[6]; //源MAC地址：发送方的MAC地址 
	unsigned long sour_ip; //源IP地址：发送方的IP地址 
	unsigned char dest_addr[6]; //目的MAC地址：ARP请求中该字段没有意义；ARP响应中为接收方的MAC地址 
	unsigned long dest_ip; //目的IP地址：ARP请求中为请求解析的IP地址；ARP响应中为接收方的IP地址
	unsigned char padding[18]; 
}; 

struct arp_packet //最终arp包结构 
{ 
	ethernet_head eth; //以太网头部 
	arp_head arp; //arp数据包头部 
}; 
#pragma pack()

/*!
\brief   iptos
\param   u_long in：
\return  类型为 char * 。
\version 1.0
\author  康  林
\date    2013/4/5 10:03:21
*/
char * iptos(u_long in);
/*!
\brief   ip6地址转换到字符串
\param   struct sockaddr * sockaddr：
\param   char * address            ：
\param   int addrlen               ：
\return  类型为 char * 。
\version 1.0
\author  康  林
\date    2013/4/5 10:02:57
*/
char * ip6tos(struct sockaddr * sockaddr, char * address, int addrlen);
/*!
\brief   打印接口信息
\param   pcap_if_t * d：
\return  类型为 void 。
\version 1.0
\author  康  林
\date    2013/4/5 10:02:39
*/
int ifprint(pcap_if_t * d, void *para);

typedef int (*IfPrintCallBack)(pcap_if_t*d, void *para);

/*!
\brief   显示接口信息
\param   无
\return  类型为 int 。
\version 1.0
\author  康  林
\date    2013/4/5 10:02:08
*/
int ListInterfaceInfomation(IfPrintCallBack callBack = NULL, void * para = NULL);

/** 
* 获得网卡的MAC地址 
* pDevName 网卡的设备名称 
*/ 
unsigned char* GetSelfMac(char* pDevName, unsigned char* mac); 

/*!
\brief   建立arp包
\param   struct arp_packet * pPacket：包指针
\param   unsigned char * pSrcMac    ：源MAC地址，6字节MAC地址数组
\param   unsigned char * pDstMac    ：目的MAC地址，6字节MAC地址数组
\param   unsigned long lSrcIp       ：源IP。4字节IP地址
\param   unsigned long lDstIp       ：目的IP
\return  类型为 int 。
\version 1.0
\author  康  林
\date    2013/4/7 13:14:06
*/
int BuildArpPacket(
    /*[in/out]*/struct arp_packet * pPacket,
	/*[in]*/unsigned char * pSrcMac,
	/*[in]*/unsigned char * pDstMac,
	/*[in]*/unsigned long lSrcIp,
	/*[in]*/unsigned long lDstIp,
	/*[in]*/unsigned short option = ARP_REQUEST/*请求类型*/
	);

/*!
\brief   建立arp包
\param   struct arp_packet * pPacket：
\param   char * pSrcMac             ：源MAC地址，字符串，例如：“1234567890ab”
\param   char * pDstMac             ：目的MAC地址
\param   char * pSrcIp              ：源IP。字符串，例如：“192.168.1.111”
\param   char * pDstIp              ：目的IP
\return  类型为 int 。
\version 1.0
\author  康  林
\date    2013/4/7 14:00:39
*/
int BuildArpPacket(
    /*[in/out]*/struct arp_packet * pPacket,
    /*[in]*/char * pSrcMac,
	/*[in]*/char * pDstMac,
	/*[in]*/char * pSrcIp,
	/*[in]*/char * pDstIp,
	/*[in]*/unsigned short option = ARP_REQUEST/*请求类型*/
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
			 char * pszHostIp, char * pszHostMac, char * pszLocalMac
			 );

/*!
\brief   根据IP得到相应的MAC地址
\param   char * pszIp       ：指定的IP。输入参数
\param   unsigned char * Mac：MAC地址。输出参数
\return  类型为 int 。
\version 1.0.0.1
\author  康  林
\date    2013-4-10 8:33:53
*/
int GetMac( /*[in]*/char * pszIp, /*[out]*/unsigned char * Mac);

/*!
\brief   把MAC数组转换成字符串。非线程安全
\param   unsigned char * pMac：MAC地址数组
\return  类型为 const char* ：转换后的MAC地址字符串。
\version 1.0.0.1
\author  康  林
\date    2013-4-10 8:37:37
*/
const char* GetMacString(unsigned char * pMac);

#endif 