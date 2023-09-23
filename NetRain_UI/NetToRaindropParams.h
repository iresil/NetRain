#pragma once

namespace N_CodeRain
{
    class NetToRaindropParams
    {
    public:
        NetToRaindropParams();
        bool getSuccess();

        void RefreshPacketCount();
        void CalculateRaindropParams();

        int getTailLength(int type);
        int getFallSecondsMultiplier(int type);
    private:
        int tcp_count;
        int udp_count;
        int local_count;

        int times_called;
        float average_tcp_count;
        float average_udp_count;
        float average_local_count;
        int max_tcp_count;
        int max_udp_count;
        int max_local_count;

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

        int local_tail_length;
        int local_fall_seconds_multiplier;
        int** local_symbols;
        int* local_change_seconds_multiplier;
        float* local_opacity;

        long timerStart;
        bool hasTimeElapsed();
        void refreshTcp(int new_tcp_count);
        void refreshUdp(int new_udp_count);
        void refreshLocal(int new_local_count);
        int calculateRaindropTail(int packet_count, int average_packet_count, int max_packet_count);
    };
}
