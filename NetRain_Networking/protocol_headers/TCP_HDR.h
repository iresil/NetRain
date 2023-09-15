#pragma once

namespace N_CodeRain_Net
{
    // TCP header
    typedef struct tcp_header
    {
        // Source port
        unsigned short source_port;
        // Destination port
        unsigned short dest_port;
        // Sequence number - 32 bits
        unsigned int sequence;
        // Acknowledgement number - 32 bits
        unsigned int acknowledge;

        // Nonce Sum Flag Added in RFC 3540.
        unsigned char ns : 1;
        // According to RFC
        unsigned char reserved_part1 : 3;
        /*
            The number of 32-bit words in the TCP header. This indicates where the data begins.
            The length of the TCP header is always a multiple of 32 bits.
        */
        unsigned char data_offset : 4;

        // Finish Flag
        unsigned char fin : 1;
        // Synchronise Flag
        unsigned char syn : 1;
        // Reset Flag
        unsigned char rst : 1;
        // Push Flag
        unsigned char psh : 1;
        // Acknowledgement Flag
        unsigned char ack : 1;
        // Urgent Flag
        unsigned char urg : 1;

        // ECN-Echo Flag
        unsigned char ecn : 1;
        // Congestion Window Reduced Flag
        unsigned char cwr : 1;

        // window
        unsigned short window;
        // checksum
        unsigned short checksum;
        // urgent pointer
        unsigned short urgent_pointer;
    } TCP_HDR;
}
