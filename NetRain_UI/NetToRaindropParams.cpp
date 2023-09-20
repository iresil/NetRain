#include "pch.h"
#include "NetToRaindropParams.h"
#include "../NetRain_Networking/Sniffer.h"

namespace N_CodeRain
{
    NetToRaindropParams::NetToRaindropParams()
    {
        this->tcp_count = 0;
        this->udp_count = 0;
        this->timerStart = GetTickCount64();

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

        if (this->hasTimeElapsed())
        {
            this->refreshTcp(tcp_count);
            this->refreshUdp(udp_count);
        }
    }

    bool NetToRaindropParams::hasTimeElapsed()
    {
        bool elapsed = false;
        long currentTime = GetTickCount64();
        long elapsed_milliseconds = currentTime - this->timerStart;
        if (elapsed_milliseconds > 6000)
        {
            this->timerStart = currentTime;
            elapsed = true;
        }
        else
        {
            elapsed = false;
        }
        return elapsed;
    }

    void NetToRaindropParams::refreshTcp(int new_tcp_count)
    {
        this->tcp_count = new_tcp_count + 4;
        this->average_tcp_count = this->tcp_count;
        this->max_tcp_count = this->tcp_count;
        this->times_called = 0;
    }

    void NetToRaindropParams::refreshUdp(int new_udp_count)
    {
        this->udp_count = new_udp_count + 4;
        this->average_udp_count = this->udp_count;
        this->max_udp_count = this->udp_count;
        this->times_called = 0;
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
            if (max_tcp_tail > 30)
            {
                max_tcp_tail = ((max_tcp_tail - 0.0) / (max_tcp_tail - 0.0)) * (30 - 10) + 10;
                min_tcp_tail = ((min_tcp_tail - 0.0) / (max_tcp_tail - 0.0)) * (30 - 10) + 10;
            }
            tcp_tail = ((this->tcp_count - 0.0) / (this->max_tcp_count - 0.0)) * (max_tcp_tail - min_tcp_tail) + min_tcp_tail;
        }
        int udp_tail = 0;
        if (this->max_udp_count > 0)
        {
            if (max_udp_tail > 30)
            {
                max_udp_tail = ((max_udp_tail - 0.0) / (max_udp_tail - 0.0)) * (30 - 10) + 10;
                min_udp_tail = ((min_udp_tail - 0.0) / (max_udp_tail - 0.0)) * (30 - 10) + 10;
            }
            udp_tail = ((this->udp_count - 0.0) / (this->max_udp_count - 0.0)) * (max_udp_tail - min_udp_tail) + min_udp_tail;
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
