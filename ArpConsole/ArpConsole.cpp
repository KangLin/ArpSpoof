// ArpConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../ArpSpoof/ArpSpoofLib.h"

 extern int gDevIndex;

int IfFindName(pcap_if_t * d, void *para)
{
	int nRet = 0;
	//if(giN
	return nRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iNum = 0;
	
	char szInterfaceName[1000];
	ListInterfaceInfomation();

	//���û�ѡ��һ������ 
	printf("Enter the interface number (1-%d):",gDevIndex); 
	scanf("%d", &iNum); 
	if(iNum <= 0)
	{
		printf("input error\n");
		return -1;
	}

	return 0;
}

