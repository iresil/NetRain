#pragma once
#include <WinSock2.h>
#include "protocol_headers/IPV4_HDR.h"
#include "protocol_headers/TCP_HDR.h"
#include "protocol_headers/UDP_HDR.h"
#include "protocol_headers/ICMP_HDR.h"

namespace N_CodeRain_Net
{
    class Sniffer
    {
    public:
        int Sniff();
    private:
        struct sockaddr_in source, dest;
        char hex[2];

        IPV4_HDR* iphdr;
        TCP_HDR* tcpheader;
        UDP_HDR* udpheader;
        ICMP_HDR* icmpheader;

        int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;

        void StartSniffing(SOCKET sniffer);
        void ProcessPacket(char* Buffer, int Size);

        void PrintIpHeader(char* Buffer);
        void PrintTcpPacket(char* Buffer, int Size);
        void PrintUdpPacket(char* Buffer, int Size);
        void PrintIcmpPacket(char* Buffer, int Size);

        void PrintData(char* data, int Size);
    };
}
