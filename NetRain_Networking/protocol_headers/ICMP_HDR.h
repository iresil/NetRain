#pragma once

namespace N_CodeRain_Net
{
    typedef struct icmp_hdr
    {
        // ICMP Error type
        BYTE type;
        // Type sub code
        BYTE code;
        USHORT checksum;
        USHORT id;
        USHORT seq;
    } ICMP_HDR;
}
