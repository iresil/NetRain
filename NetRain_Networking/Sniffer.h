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
        bool getSuccess();

        int getTcpCount();
        int getUdpCount();
        int getLocalCount();
    private:
        Sniffer();
        static Sniffer* instancePtr;
        volatile bool success;

        ~Sniffer();
        Sniffer(const Sniffer&) = delete;
        Sniffer(Sniffer& other) = delete;
        Sniffer& operator=(const Sniffer&) = delete;

        struct hostent* host;

        char hostname[100];
        volatile int tcp, udp, icmp, others, igmp, local, total;
        int network_interface_count;
        bool run_thread;
        bool debug;

        void SetupNetworkInterfaces();
        static void Sniff(void* ignored);
        int Sniff(int in);

        void StartSniffing(SOCKET sniffer);
        void ProcessPacket(char* Buffer, int Size);

        void PrintIpHeader(char* Buffer);
        void PrintTcpPacket(char* Buffer, int Size);
        void PrintUdpPacket(char* Buffer, int Size);
        void PrintIcmpPacket(char* Buffer, int Size);

        void PrintData(char* data, int Size);
    };
}
