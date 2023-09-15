#pragma once

namespace N_CodeRain_Net
{
    typedef struct udp_hdr
    {
        // Source port no.
        unsigned short source_port;
        // Destination port no.
        unsigned short dest_port;
        // UDP packet length
        unsigned short udp_length;
        // UDP checksum (optional)
        unsigned short udp_checksum;
    } UDP_HDR;
}
