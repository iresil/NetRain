#pragma once

namespace N_CodeRain_Net
{
    typedef struct ip_hdr
    {
        // 4-bit header length (in 32-bit words) normally=5 (Means 20 Bytes may be 24 also)
        unsigned char ip_header_len : 4;
        // 4-bit IPv4 version
        unsigned char ip_version : 4;
        // IP type of service
        unsigned char ip_tos;
        // Total length
        unsigned short ip_total_length;
        // Unique identifier
        unsigned short ip_id;

        // Fragment offset field
        unsigned char ip_frag_offset : 5;

        unsigned char ip_more_fragment : 1;
        unsigned char ip_dont_fragment : 1;
        unsigned char ip_reserved_zero : 1;

        // Fragment offset
        unsigned char ip_frag_offset1;

        // Time to live
        unsigned char ip_ttl;
        // Protocol(TCP, UDP etc)
        unsigned char ip_protocol;
        // IP checksum
        unsigned short ip_checksum;
        // Source address
        unsigned int ip_srcaddr;
        // Destination address
        unsigned int ip_destaddr;
    } IPV4_HDR;
}
