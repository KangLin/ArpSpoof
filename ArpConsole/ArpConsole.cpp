// ArpConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../ArpSpoof/ArpSpoofLib.h"

extern int gDevIndex;
char szInterfaceName[1000];

int IfFindName(pcap_if_t * d, void *para)
{
	int nRet = 0;
	//if(giN
	return nRet;
}

void printInfo(int argc, _TCHAR* argv[])
{
	printf("User: %s GatewayIp HostIp [GatewayMac] [HostMac]\n", argv[0]);
}

int GetDev(pcap_if_t *d, void *para)
{
	pcap_addr_t *a;
	char ip6str[128];
	int* nNum = (int*)para;
	if(gDevIndex == *nNum)
	{
		strcpy(szInterfaceName, d->name);
		return -1;
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iNum = 0;

	if(argc != 2 && argc !=3)
	{
		printInfo(argc, argv);
		return -1;
	}

	ListInterfaceInfomation();

	//请用户选择一个网卡 
	printf("Enter the interface number (1-%d):",gDevIndex); 
	scanf("%d", &iNum); 
	if(iNum <= 0)
	{
		printf("input error\n");
		return -1;
	}

	ListInterfaceInfomation(GetDev, &iNum);

	ArpSpoof(
		szInterfaceName,
		argv[1],
		argv[2],
		1000 /*ms*/
		);

	return 0;
}
