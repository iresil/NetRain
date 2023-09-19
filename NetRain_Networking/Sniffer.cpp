#include "pch.h"
#include "Sniffer.h"
#include <string>
//#pragma comment(lib,"WS2_32")

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)

namespace N_CodeRain_Net
{
    Sniffer* Sniffer::instancePtr = NULL;

    Sniffer::Sniffer()
    {
        this->success = false;

        this->tcp = 0;
        this->udp = 0;
        this->icmp = 0;
        this->others = 0;
        this->igmp = 0;
        this->total = 0;

        this->debug = false;

        this->run_thread = true;

        SetupNetworkInterfaces();

        for (int i = 0; i < this->network_interface_count; ++i)
        {
            _beginthread(Sniff, 0, (void*)i);
        }
    }

    Sniffer::~Sniffer()
    {
        this->run_thread = false;
        delete[] this->hostname;
        delete this->local;
    }

    Sniffer* Sniffer::getInstance() {
        if (instancePtr == NULL)
        {
            instancePtr = new Sniffer();
            return instancePtr;
        }
        else
        {
            return instancePtr;
        }
    }

    void Sniffer::Sniff(void* ignored)
    {
        getInstance()->Sniff((int)ignored);
        _endthread();
    }

    bool Sniffer::getSuccess()
    {
        return this->success;
    }

    int Sniffer::getTcpCount()
    {
        int result = this->tcp;
        this->tcp = 0;
        return result;
    }

    int Sniffer::getUdpCount()
    {
        int result = this->udp;
        this->udp = 0;
        return result;
    }

    void Sniffer::SetupNetworkInterfaces()
    {
        // Initialize Winsock
        WSADATA wsa;
        OutputDebugStringW(L"\nInitialising Winsock...");
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        {
            OutputDebugStringW(L"WSAStartup() failed.\n");
            WSACleanup();
            return;
        }
        OutputDebugStringW(L"Initialised");

        wchar_t* wString = new wchar_t[4096];
        // Retrieve the local hostname
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
        {
            OutputDebugStringW((std::wstring(L"Error: ") + std::to_wstring(WSAGetLastError()) + std::wstring(L"\n")).c_str());
            WSACleanup();
            return;
        }
        MultiByteToWideChar(CP_ACP, 0, hostname, -1, wString, 4096);
        OutputDebugStringW((std::wstring(L"\nHost name: ") + wString + std::wstring(L" \n")).c_str());

        // Retrieve the available IPs of the local host
        local = gethostbyname(hostname);
        OutputDebugStringW(L"\nAvailable Network Interfaces: \n");
        if (local == NULL)
        {
            OutputDebugStringW((std::wstring(L"Error: ") + std::to_wstring(WSAGetLastError()) + std::wstring(L"\n")).c_str());
            WSACleanup();
            return;
        }

        for (int i = 0; local->h_addr_list[i] != 0; ++i)
        {
            network_interface_count++;
        }
    }

    int Sniffer::Sniff(int in)
    {
        SOCKET sniffer;
        struct in_addr addr;

        wchar_t* wString = new wchar_t[4096];

        // Create a RAW Socket
        OutputDebugStringW(L"\nCreating RAW Socket...");
        sniffer = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
        if (sniffer == INVALID_SOCKET)
        {
            OutputDebugStringW(L"Failed to create raw socket.\n");
            WSACleanup();
            return 1;
        }
        OutputDebugStringW(L"Created.");

        memcpy(&addr, local->h_addr_list[in], sizeof(struct in_addr));
        MultiByteToWideChar(CP_ACP, 0, inet_ntoa(addr), -1, wString, 4096);
        OutputDebugStringW((std::wstring(L"Interface Number: ") + std::to_wstring(in) + std::wstring(L" Address: ") + wString + std::wstring(L" Type: ") + std::to_wstring(local->h_addrtype)).c_str());

        // Start sniffing from the first interface
        struct sockaddr_in dest = *(new sockaddr_in());
        memset(&dest, 0, sizeof(dest));

        if (local->h_addr_list[in] != nullptr)
        {
            memcpy(&dest.sin_addr.s_addr, local->h_addr_list[in], sizeof(dest.sin_addr.s_addr));
            dest.sin_family = AF_INET;
            dest.sin_port = in;

            OutputDebugStringW(L"\nBinding socket to local system and port 0 ...");
            if (bind(sniffer, (struct sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR)
            {
                MultiByteToWideChar(CP_ACP, 0, inet_ntoa(addr), -1, wString, 4096);
                OutputDebugStringW((std::wstring(L"bind(") + wString + std::wstring(L") failed.\n")).c_str());
                WSACleanup();
                return 1;
            }
            OutputDebugStringW(L"Binding successful");

            // Enable this socket with the power to sniff: SIO_RCVALL is the key Receive ALL
            int j = 1;
            OutputDebugStringW(L"\nSetting socket to sniff...");
            if (WSAIoctl(sniffer, SIO_RCVALL, &j, sizeof(j), 0, 0, (LPDWORD)&in, 0, 0) == SOCKET_ERROR)
            {
                OutputDebugStringW(L"WSAIoctl() failed.\n");
                WSACleanup();
                return 1;
            }
            OutputDebugStringW(L"Socket set.");

            OutputDebugStringW(L"\nStarted Sniffing\n");
            OutputDebugStringW(L"Packet Capture Statistics...\n");

            StartSniffing(sniffer);

            closesocket(sniffer);
            WSACleanup();
        }

        return 0;
    }

    void Sniffer::StartSniffing(SOCKET sniffer)
    {
        char* Buffer = (char*)malloc(65536);
        int mangobyte;

        if (Buffer == NULL)
        {
            OutputDebugStringW(L"malloc() failed.\n");
            return;
        }

        do
        {
            mangobyte = recvfrom(sniffer, Buffer, 65536, 0, 0, 0);

            if (mangobyte > 0)
            {
                ProcessPacket(Buffer, mangobyte);
                this->success = true;
            }
            else
            {
                OutputDebugStringW(L"recvfrom() failed.\n");
            }
        } while (mangobyte > 0 && run_thread);

        free(Buffer);
    }

    void Sniffer::ProcessPacket(char* Buffer, int Size)
    {
        IPV4_HDR* iphdr = (IPV4_HDR*)Buffer;
        ++total;

        switch (iphdr->ip_protocol)
        {
        case 1:
            ++icmp;
            PrintIcmpPacket(Buffer, Size);
            break;

        case 2:
            ++igmp;
            break;

        case 6:
            ++tcp;
            PrintTcpPacket(Buffer, Size);
            break;

        case 17:
            ++udp;
            PrintUdpPacket(Buffer, Size);
            break;

        default:
            ++others;
            break;
        }

        if (debug)
        {
            OutputDebugStringW((std::wstring(L"TCP: ") + std::to_wstring(tcp) + std::wstring(L" UDP: ") + std::to_wstring(udp)
                + std::wstring(L" ICMP: ") + std::to_wstring(icmp) + std::wstring(L" IGMP: ") + std::to_wstring(igmp)
                + std::wstring(L" Others: ") + std::to_wstring(others) + std::wstring(L" Total: ") + std::to_wstring(total) + std::wstring(L"\r")).c_str());
        }
    }

    void Sniffer::PrintIpHeader(char* Buffer)
    {
        unsigned short iphdrlen;

        IPV4_HDR* iphdr = (IPV4_HDR*)Buffer;
        iphdrlen = iphdr->ip_header_len * 4;

        struct sockaddr_in source = *(new sockaddr_in());
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iphdr->ip_srcaddr;

        struct sockaddr_in dest = *(new sockaddr_in());
        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = iphdr->ip_destaddr;

        if (debug)
        {
            wchar_t* wString = new wchar_t[4096];

            OutputDebugStringW(L"\n");
            OutputDebugStringW(L"IP Header\n");
            OutputDebugStringW((std::wstring(L" |-IP Version: ") + std::to_wstring((unsigned int)iphdr->ip_version) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-IP Header Length: ") + std::to_wstring((unsigned int)iphdr->ip_header_len) + std::wstring(L" DWORDS or ")
                + std::to_wstring(((unsigned int)(iphdr->ip_header_len)) * 4) + std::wstring(L" Bytes\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Type Of Service: ") + std::to_wstring((unsigned int)iphdr->ip_tos) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-IP Total Length: ") + std::to_wstring(ntohs(iphdr->ip_total_length)) + std::wstring(L" Bytes(Size of Packet)\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Identification: ") + std::to_wstring(ntohs(iphdr->ip_id)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Reserved ZERO Field: ") + std::to_wstring((unsigned int)iphdr->ip_reserved_zero) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Dont Fragment Field: ") + std::to_wstring((unsigned int)iphdr->ip_dont_fragment) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-More Fragment Field: ") + std::to_wstring((unsigned int)iphdr->ip_more_fragment) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-TTL: ") + std::to_wstring((unsigned int)iphdr->ip_ttl) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Protocol: ") + std::to_wstring((unsigned int)iphdr->ip_protocol) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Checksum: ") + std::to_wstring(ntohs(iphdr->ip_checksum)) + std::wstring(L"\n")).c_str());
            MultiByteToWideChar(CP_ACP, 0, inet_ntoa(source.sin_addr), -1, wString, 4096);
            OutputDebugStringW((std::wstring(L" |-Source IP: ") + wString + std::wstring(L"\n")).c_str());
            MultiByteToWideChar(CP_ACP, 0, inet_ntoa(dest.sin_addr), -1, wString, 4096);
            OutputDebugStringW((std::wstring(L" |-Destination IP: ") + wString + std::wstring(L"\n")).c_str());
        }
    }

    void Sniffer::PrintTcpPacket(char* Buffer, int Size)
    {
        unsigned short iphdrlen;

        IPV4_HDR* iphdr = (IPV4_HDR*)Buffer;
        iphdrlen = iphdr->ip_header_len * 4;

        TCP_HDR* tcpheader = (TCP_HDR*)(Buffer + iphdrlen);

        if (debug)
        {
            OutputDebugStringW(L"\n\n***********************TCP Packet*************************\n");

            PrintIpHeader(Buffer);

            OutputDebugStringW(L"\n");
            OutputDebugStringW(L"TCP Header\n");
            OutputDebugStringW((std::wstring(L" |-Source Port: ") + std::to_wstring(ntohs(tcpheader->source_port)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Destination Port: ") + std::to_wstring(ntohs(tcpheader->dest_port)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Sequence Number: ") + std::to_wstring(ntohl(tcpheader->sequence)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Acknowledge Number: ") + std::to_wstring(ntohl(tcpheader->acknowledge)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Header Length: ") + std::to_wstring((unsigned int)tcpheader->data_offset) + std::wstring(L" DWORDS or ")
                + std::to_wstring((unsigned int)tcpheader->data_offset * 4) + std::wstring(L" BYTES\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-CWR Flag: ") + std::to_wstring((unsigned int)tcpheader->cwr) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-ECN Flag: ") + std::to_wstring((unsigned int)tcpheader->ecn) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Urgent Flag: ") + std::to_wstring((unsigned int)tcpheader->urg) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Acknowledgement Flag: ") + std::to_wstring((unsigned int)tcpheader->ack) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Push Flag: ") + std::to_wstring((unsigned int)tcpheader->psh) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Reset Flag: ") + std::to_wstring((unsigned int)tcpheader->rst) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Synchronise Flag: ") + std::to_wstring((unsigned int)tcpheader->syn) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Finish Flag: ") + std::to_wstring((unsigned int)tcpheader->fin) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Window: ") + std::to_wstring(ntohs(tcpheader->window)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Checksum: ") + std::to_wstring(ntohs(tcpheader->checksum)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Urgent Pointer: ") + std::to_wstring(tcpheader->urgent_pointer) + std::wstring(L"\n")).c_str());
            OutputDebugStringW(L"\n");
            OutputDebugStringW(L" DATA Dump ");
            OutputDebugStringW(L"\n");

            OutputDebugStringW(L"IP Header\n");
            PrintData(Buffer, iphdrlen);

            OutputDebugStringW(L"TCP Header\n");
            PrintData(Buffer + iphdrlen, tcpheader->data_offset * 4);

            OutputDebugStringW(L"Data Payload\n");
            PrintData(Buffer + iphdrlen + tcpheader->data_offset * 4
                , (Size - tcpheader->data_offset * 4 - iphdr->ip_header_len * 4));

            OutputDebugStringW(L"\n###########################################################");
        }
    }

    void Sniffer::PrintUdpPacket(char* Buffer, int Size)
    {
        unsigned short iphdrlen;

        IPV4_HDR* iphdr = (IPV4_HDR*)Buffer;
        iphdrlen = iphdr->ip_header_len * 4;

        UDP_HDR* udpheader = (UDP_HDR*)(Buffer + iphdrlen);

        if (debug)
        {
            OutputDebugStringW(L"\n\n***********************UDP Packet*************************\n");

            PrintIpHeader(Buffer);

            OutputDebugStringW(L"\nUDP Header\n");
            OutputDebugStringW((std::wstring(L" |-Source Port: ") + std::to_wstring(ntohs(udpheader->source_port)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Destination Port: ") + std::to_wstring(ntohs(udpheader->dest_port)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-UDP Length: ") + std::to_wstring(ntohs(udpheader->udp_length)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-UDP Checksum: ") + std::to_wstring(ntohs(udpheader->udp_checksum)) + std::wstring(L"\n")).c_str());
        
            OutputDebugStringW(L"\n");
            OutputDebugStringW(L"IP Header\n");

            PrintData(Buffer, iphdrlen);

            OutputDebugStringW(L"UDP Header\n");

            PrintData(Buffer + iphdrlen, sizeof(UDP_HDR));

            OutputDebugStringW(L"Data Payload\n");

            PrintData(Buffer + iphdrlen + sizeof(UDP_HDR), (Size - sizeof(UDP_HDR) - iphdr->ip_header_len * 4));

            OutputDebugStringW(L"\n###########################################################");
        }
    }

    void Sniffer::PrintIcmpPacket(char* Buffer, int Size)
    {
        unsigned short iphdrlen;

        IPV4_HDR* iphdr = (IPV4_HDR*)Buffer;
        iphdrlen = iphdr->ip_header_len * 4;

        ICMP_HDR* icmpheader = (ICMP_HDR*)(Buffer + iphdrlen);

        if (debug)
        {
            OutputDebugStringW(L"\n\n***********************ICMP Packet*************************\n");
            PrintIpHeader(Buffer);

            OutputDebugStringW(L"\n");

            OutputDebugStringW(L"ICMP Header\n");
            OutputDebugStringW((std::wstring(L" |-Type: ") + std::to_wstring((unsigned int)(icmpheader->type)) + std::wstring(L"\n")).c_str());

            if ((unsigned int)(icmpheader->type) == 11)
            {
                OutputDebugStringW(L" (TTL Expired)\n");
            }
            else if ((unsigned int)(icmpheader->type) == 0)
            {
                OutputDebugStringW(L" (ICMP Echo Reply)\n");
            }

            OutputDebugStringW((std::wstring(L" |-Code: ") + std::to_wstring((unsigned int)(icmpheader->code)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Checksum: ") + std::to_wstring(ntohs(icmpheader->checksum)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-ID: ") + std::to_wstring(ntohs(icmpheader->id)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW((std::wstring(L" |-Sequence: ") + std::to_wstring(ntohs(icmpheader->seq)) + std::wstring(L"\n")).c_str());
            OutputDebugStringW(L"\n");

            OutputDebugStringW(L"IP Header\n");
            PrintData(Buffer, iphdrlen);

            OutputDebugStringW(L"UDP Header\n");
            PrintData(Buffer + iphdrlen, sizeof(ICMP_HDR));

            OutputDebugStringW(L"Data Payload\n");
            PrintData(Buffer + iphdrlen + sizeof(ICMP_HDR), (Size - sizeof(ICMP_HDR) - iphdr->ip_header_len * 4));

            OutputDebugStringW(L"\n###########################################################");
        }
    }

    // Print the hex values of the data.
    void Sniffer::PrintData(char* data, int Size)
    {
        if (debug)
        {
            char a, line[17], c;
            int j;

            for (int i = 0; i < Size; i++)
            {
                c = data[i];

                // Print the hex value for every character, with a space. Important to make unsigned
                OutputDebugStringW((std::wstring(L" ") + std::to_wstring(0xFF & (unsigned char)c)).c_str());

                // Add the character to data line. Important to make unsigned
                a = (c >= 32 && c <= 128) ? (unsigned char)c : '.';

                line[i % 16] = a;

                // If last character of a line, then print the line - 16 characters in 1 line
                if ((i != 0 && (i + 1) % 16 == 0) || i == Size - 1)
                {
                    line[i % 16 + 1] = '\0';

                    // Print a big gap of 10 characters between hex and characters
                    OutputDebugStringW(L"          ");

                    // Print additional spaces for last lines which might be less than 16 characters in length
                    for (j = strlen(line); j < 16; j++)
                    {
                        OutputDebugStringW(L"   ");
                    }

                    wchar_t* wString = new wchar_t[4096];
                    MultiByteToWideChar(CP_ACP, 0, line, -1, wString, 4096);
                    OutputDebugStringW(wString);
                    OutputDebugStringW(L"\n");
                }
            }

            OutputDebugStringW(L"\n");
        }
    }
}
