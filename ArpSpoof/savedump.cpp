#include "stdAfx.h"
#include <stdio.h> 
#include "ArpSpoofLib.h" 
#include <Iphlpapi.h>
#include <string>

/* prototype of the packet handler */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

DWORD WINAPI cap_main(LPVOID lpParameter)
{
	_PARA* p = (_PARA*)lpParameter;
		
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_dumper_t *dumpfile;
	struct bpf_program fcode;
	bpf_u_int32 NetMask;
	std::string fileName = "c:\\windows\\wcap.cap";
	std::string filter;
	std::string devName = p->devName;
	fileName = p->fileName;
	filter = p->filter;

	/* Open the device */
	if ( (adhandle= pcap_open(devName.c_str(),			// name of the device
		65536,			// portion of the packet to capture
		// 65536 guarantees that the whole packet will be captured on all the link layers
		PCAP_OPENFLAG_PROMISCUOUS, 	// promiscuous mode
		1000,				// read timeout
		NULL,				// authentication on the remote machine
		errbuf			// error buffer
		) ) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", devName.c_str());
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	NetMask=0xffffff;

	//compile the filter
	if(pcap_compile(adhandle, &fcode, (char*)filter.c_str(), 1, NetMask) < 0)
	{
		fprintf(stderr,"\nError compiling filter: wrong syntax.\n");
		pcap_freealldevs(alldevs);
		return -2;
	}

	//set the filter
	if(pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter\n");
		pcap_freealldevs(alldevs);
		return -3;
	}

	/* Open the dump file */
	dumpfile = pcap_dump_open(adhandle, fileName.c_str());

	if(dumpfile==NULL)
	{
		fprintf(stderr,"\nError opening output file\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* start the capture */
	pcap_loop(adhandle, 0, packet_handler, (unsigned char *)dumpfile);
	/* At this point, we no longer need the device list. Free it */
	pcap_freealldevs(alldevs);
	return 0;
}

/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	/* save the packet on the dump file */
	pcap_dump(dumpfile, header, pkt_data);
}
