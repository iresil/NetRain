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
        static Sniffer* getInstance();
    private:
        Sniffer();
        static Sniffer* instancePtr;

        ~Sniffer();
        Sniffer(const Sniffer&) = delete;
        Sniffer(Sniffer& other) = delete;
        Sniffer& operator=(const Sniffer&) = delete;

        struct sockaddr_in source, dest;

        IPV4_HDR* iphdr;
        TCP_HDR* tcpheader;
        UDP_HDR* udpheader;
        ICMP_HDR* icmpheader;

        int tcp, udp, icmp, others, igmp, total;
        bool run_thread;

        static void Sniff(void* ignored);
        int Sniff();

        void StartSniffing(SOCKET sniffer);
        void ProcessPacket(char* Buffer, int Size);

        void PrintIpHeader(char* Buffer);
        void PrintTcpPacket(char* Buffer, int Size);
        void PrintUdpPacket(char* Buffer, int Size);
        void PrintIcmpPacket(char* Buffer, int Size);

        void PrintData(char* data, int Size);
    };
}
