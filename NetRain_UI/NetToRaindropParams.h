#pragma once

namespace N_CodeRain
{
    class NetToRaindropParams
    {
    public:
        NetToRaindropParams();

        void RefreshPacketCount();
        void CalculateRaindropParams();

        int getTailLength(int type);
        int getFallSecondsMultiplier(int type);
    private:
        int tcp_count;
        int udp_count;

        int times_called;
        float average_tcp_count;
        float average_udp_count;
        int max_tcp_count;
        int max_udp_count;

        int tcp_tail_length;
        int tcp_fall_seconds_multiplier;
        int** tcp_symbols;
        int* tcp_change_seconds_multiplier;
        float* tcp_opacity;

        int udp_tail_length;
        int udp_fall_seconds_multiplier;
        int** udp_symbols;
        int* udp_change_seconds_multiplier;
        float* udp_opacity;
    };
}
