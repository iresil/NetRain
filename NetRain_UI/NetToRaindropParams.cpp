#include "pch.h"
#include "NetToRaindropParams.h"
#include "../NetRain_Networking/Sniffer.h"

namespace N_CodeRain
{
    NetToRaindropParams::NetToRaindropParams()
    {
        this->tcp_count = 0;
        this->udp_count = 0;

        this->times_called = 0;
        this->average_tcp_count = 0;
        this->average_udp_count = 0;
        this->max_tcp_count = 0;
        this->max_udp_count = 0;

        this->tcp_tail_length = 1;
        this->tcp_fall_seconds_multiplier = 1;
        this->tcp_symbols = nullptr;
        this->tcp_change_seconds_multiplier = nullptr;
        this->tcp_opacity = nullptr;

        this->udp_tail_length = 1;
        this->udp_fall_seconds_multiplier = 1;
        this->udp_symbols = nullptr;
        this->udp_change_seconds_multiplier = nullptr;
        this->udp_opacity = nullptr;
    }

    bool NetToRaindropParams::getSuccess()
    {
        return N_CodeRain_Net::Sniffer::getInstance()->getSuccess();
    }

    void NetToRaindropParams::RefreshPacketCount()
    {
        this->times_called++;

        int tcp_count = N_CodeRain_Net::Sniffer::getInstance()->getTcpCount();
        int udp_count = N_CodeRain_Net::Sniffer::getInstance()->getUdpCount();
        this->tcp_count += tcp_count;
        this->udp_count += udp_count;

        this->average_tcp_count = (this->average_tcp_count * (this->times_called - 1) + this->tcp_count) / this->times_called;
        this->average_udp_count = (this->average_udp_count * (this->times_called - 1) + this->udp_count) / this->times_called;

        this->max_tcp_count = (this->tcp_count > this->max_tcp_count) ? this->tcp_count : this->max_tcp_count;
        this->max_udp_count = (this->udp_count > this->max_udp_count) ? this->udp_count : this->max_udp_count;

        if (this->max_tcp_count > 30)
        {
            this->tcp_count = tcp_count;
            this->average_tcp_count = this->tcp_count;
            this->max_tcp_count = this->tcp_count;
            this->times_called = 0;
        }
        if (this->max_udp_count > 30)
        {
            this->udp_count = udp_count;
            this->average_udp_count = this->udp_count;
            this->max_udp_count = this->udp_count;
            this->times_called = 0;
        }
    }

    void NetToRaindropParams::CalculateRaindropParams()
    {
        int max_tcp_tail = (this->average_tcp_count / 20) * 30;
        int max_udp_tail = (this->average_udp_count / 20) * 30;

        int min_tcp_tail = (this->average_tcp_count / 20) * 10;
        int min_udp_tail = (this->average_udp_count / 20) * 10;

        int tcp_tail = 0;
        if (this->max_tcp_count > 0)
        {
            tcp_tail = ((this->tcp_count - 0) / (this->max_tcp_count - 0)) * (max_tcp_tail - min_tcp_tail) + min_tcp_tail;
        }
        int udp_tail = 0;
        if (this->max_udp_count > 0)
        {
            udp_tail = ((this->udp_count - 0) / (this->max_udp_count - 0)) * (max_udp_tail - min_udp_tail) + min_udp_tail;
        }
        this->tcp_tail_length = tcp_tail;
        this->udp_tail_length = udp_tail;
    }

    int NetToRaindropParams::getTailLength(int type)
    {
        if (type == 0)
        {
            return this->tcp_tail_length;
        }
        else if (type == 1)
        {
            return this->udp_tail_length;
        }
        else
        {
            return 0;
        }
    }

    int NetToRaindropParams::getFallSecondsMultiplier(int type)
    {
        if (type == 0)
        {
            return this->tcp_fall_seconds_multiplier;
        }
        else if (type == 1)
        {
            return this->udp_fall_seconds_multiplier;
        }
        else
        {
            return 0;
        }
    }
}
