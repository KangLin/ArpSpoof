// AllSpoof.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../ArpSpoof/ArpSpoofLib.h"

int Spoof()
{
	int nRet = 0;
	//struct arp_packet packet;
	//pcap_t * pHandler;
	//char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
	//unsigned char gatewayMac[6] = {0};
	//unsigned char hostMac[6] = {0};
	//unsigned char localMac[6] = {0};

	//GetSelfMac(pszInterfaceName, localMac);
	//GetMac(pszGatewayIp, gatewayMac);
	//GetMac(pszHostIp, hostMac);

	///* ������ */ 
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

	////�����غ������ֱ�ʱ������ƭ��
	//while(1)
	//{
	//	BuildArpReply(&packet, localMac, hostMac, pszGatewayIp, pszHostIp);
	//	if(pcap_sendpacket(pHandler, (const u_char * )&packet, 60) == -1)
	//	{
	//		LOG_DEBUG("pcap_sendpacket send arp spoof to host error.\n");
	//	}

	//	Sleep(nInterval);
	//} // ���� while(1)

	//pcap_close(pHandler); 
	return nRet;
}


int main(int argc,char* argv[]){ 
	//pcap_if_t *alldevs;               //ȫ�������б� 
	//pcap_if_t *d;                     //һ������ 
	//int inum;                         //�û�ѡ���������� 
	//int i=0;                          //ѭ������ 
	//pcap_t *adhandle;                 //һ��pcapʵ�� 
	//char errbuf[PCAP_ERRBUF_SIZE];    //���󻺳��� 
	//unsigned char mac[6], dstMac[6];             //����MAC��ַ 
	//struct arp_packet  packet;        //ARP�� 
	//unsigned long fakeIp;             //Ҫαװ�ɵ�IP��ַ 
	//pcap_addr_t *pAddr;               //������ַ 
	//unsigned long ip;                 //IP��ַ 
	//unsigned long netmask;            //�������� 

	//if(argc!=2){ 
	//	printf("Usage: %s inet_addr\n",argv[0]); 
	//	return -1; 
	//} 

	////�Ӳ����б���Ҫαװ��IP��ַ 
	//fakeIp = inet_addr(argv[1]); 
	//if(INADDR_NONE==fakeIp){ 
	//	fprintf(stderr,"Invalid IP: %s\n",argv[1]); 
	//	return -1; 
	//} 

	///* ��ñ��������б� */ 
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) 
	//{ 
	//	fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf); 
	//	exit(1); 
	//} 

	///* ��ӡ�����б� */ 
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
	////���û�з������� 
	//if(i==0) 
	//{ 
	//	printf("\nNo interfaces found! Make sure WinPcap is installed.\n"); 
	//	return -1; 
	//} 
	////���û�ѡ��һ������ 
	//printf("Enter the interface number (1-%d):",i); 
	//scanf("%d", &inum); 

	////����û�ѡ���������ų�����Ч��Χ�����˳� 
	//if(inum < 1 || inum > i) 
	//{ 
	//	printf("\nInterface number out of range.\n"); 
	//	/* Free the device list */ 
	//	pcap_freealldevs(alldevs); 
	//	return -1; 
	//} 

	///* �ƶ�ָ�뵽�û�ѡ������� */ 
	//for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++); 

	//GetSelfMac(d->name, mac); //+8��ȥ��"rpcap://" 

	//printf("����ARP��ƭ��������(%.2X-%.2X-%.2X-%.2X-%.2X-%.2X) ��ͼαװ��%s\n", 
	//	mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],argv[1]); 



	///* ������ */ 
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
	//	//�õ��û�ѡ���������һ��IP��ַ 
	//	ip = ((struct sockaddr_in *)pAddr->addr)->sin_addr.s_addr; 
	//	//�õ���IP��ַ��Ӧ���������� 
	//	netmask = ((struct sockaddr_in *)(pAddr->netmask))->sin_addr.S_un.S_addr; 
	//	if (!ip || !netmask){ 
	//		continue; 
	//	} 
	//	//�������IP��Ҫαװ��IP�Ƿ���ͬһ������ 
	//	if((ip&netmask)!=(fakeIp&netmask)){ 
	//		continue; //�������һ������������������ַ�б� 
	//	} 

	//	unsigned long netsize = ntohl(~netmask); //������������
	//	unsigned long net = ip & netmask; //������ַ 

	//	for(unsigned long n=1; n<netsize; n++){ 
	//		//��į������IP��ַ�������ֽ�˳�� 
	//		unsigned long destIp = net | htonl(n); 
	//		//�����ٵ�ARP��������ﵽ����αװ�ɸ�����IP��ַ��Ŀ�� 
	//		packet = BuildArpPacket(mac,fakeIp,destIp); 
	//		if(pcap_sendpacket(adhandle, (const u_char*)packet, 60)==-1){ 
	//			fprintf(stderr,"pcap_sendpacket error.\n"); 
	//		} 
	//	}
	//}

	return 0;
}
