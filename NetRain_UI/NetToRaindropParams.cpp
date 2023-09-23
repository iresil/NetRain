#include "pch.h"
#include "NetToRaindropParams.h"
#include "../NetRain_Networking/Sniffer.h"
#include "../NetRain_Common/Consts.h"
#include "../NetRain_Common/Enums.h"

namespace N_CodeRain
{
    NetToRaindropParams::NetToRaindropParams()
    {
        this->tcp_count = 0;
        this->udp_count = 0;
        this->local_count = 0;
        this->timerStart = GetTickCount64();

        this->times_called = 0;
        this->average_tcp_count = 0;
        this->average_udp_count = 0;
        this->average_local_count = 0;
        this->max_tcp_count = 0;
        this->max_udp_count = 0;
        this->max_local_count = 0;

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

        this->local_tail_length = 1;
        this->local_fall_seconds_multiplier = 1;
        this->local_symbols = nullptr;
        this->local_change_seconds_multiplier = nullptr;
        this->local_opacity = nullptr;
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
        int local_count = N_CodeRain_Net::Sniffer::getInstance()->getLocalCount();
        this->tcp_count += tcp_count;
        this->udp_count += udp_count;
        this->local_count += local_count;

        this->average_tcp_count = (this->average_tcp_count * (this->times_called - 1) + this->tcp_count) / this->times_called;
        this->average_udp_count = (this->average_udp_count * (this->times_called - 1) + this->udp_count) / this->times_called;
        this->average_local_count = (this->average_local_count * (this->times_called - 1) + this->local_count) / this->times_called;

        this->max_tcp_count = (this->tcp_count > this->max_tcp_count) ? this->tcp_count : this->max_tcp_count;
        this->max_udp_count = (this->udp_count > this->max_udp_count) ? this->udp_count : this->max_udp_count;
        this->max_local_count = (this->local_count > this->max_local_count) ? this->local_count : this->max_local_count;

        if (this->hasTimeElapsed())
        {
            this->refreshTcp(tcp_count);
            this->refreshUdp(udp_count);
            this->refreshLocal(local_count);
        }
    }

    bool NetToRaindropParams::hasTimeElapsed()
    {
        bool elapsed = false;
        long currentTime = GetTickCount64();
        long elapsed_milliseconds = currentTime - this->timerStart;
        if (elapsed_milliseconds > PACKET_COUNT_REFRESH_MILLISECONDS)
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
        this->tcp_count = new_tcp_count + MIN_PACKET_COUNT_AFTER_REFRESH;
        this->average_tcp_count = this->tcp_count;
        this->max_tcp_count = this->tcp_count;
        this->times_called = 0;
    }

    void NetToRaindropParams::refreshUdp(int new_udp_count)
    {
        this->udp_count = new_udp_count + MIN_PACKET_COUNT_AFTER_REFRESH;
        this->average_udp_count = this->udp_count;
        this->max_udp_count = this->udp_count;
        this->times_called = 0;
    }

    void NetToRaindropParams::refreshLocal(int new_local_count)
    {
        this->local_count = new_local_count + MIN_PACKET_COUNT_AFTER_REFRESH;
        this->average_local_count = this->local_count;
        this->max_local_count = this->local_count;
        this->times_called = 0;
    }

    void NetToRaindropParams::CalculateRaindropParams()
    {
        this->tcp_tail_length = calculateRaindropTail(this->tcp_count, this->average_tcp_count, this->max_tcp_count);
        this->udp_tail_length = calculateRaindropTail(this->udp_count, this->average_udp_count, this->max_udp_count);
        this->local_tail_length = calculateRaindropTail(this->local_count, this->average_local_count, this->max_local_count);
    }

    int NetToRaindropParams::calculateRaindropTail(int packet_count, int average_packet_count, int max_packet_count)
    {
        int max_tail = (average_packet_count / ((RAINDROP_MAX_TAIL_SIZE + RAINDROP_MIN_TAIL_SIZE) / 2.0)) * RAINDROP_MAX_TAIL_SIZE;
        int min_tail = (average_packet_count / ((RAINDROP_MAX_TAIL_SIZE + RAINDROP_MIN_TAIL_SIZE) / 2.0)) * RAINDROP_MIN_TAIL_SIZE;

        int tail = 0;
        if (max_packet_count > 0)
        {
            if (max_tail > RAINDROP_MAX_TAIL_SIZE)
            {
                max_tail = ((max_tail - 0.0) / (max_tail - 0.0)) * (RAINDROP_MAX_TAIL_SIZE - RAINDROP_MIN_TAIL_SIZE) + RAINDROP_MIN_TAIL_SIZE;
                min_tail = ((min_tail - 0.0) / (max_tail - 0.0)) * (RAINDROP_MAX_TAIL_SIZE - RAINDROP_MIN_TAIL_SIZE) + RAINDROP_MIN_TAIL_SIZE;
            }
            tail = ((packet_count - 0.0) / (max_packet_count - 0.0)) * (max_tail - min_tail) + min_tail;
        }

        return tail;
    }

    int NetToRaindropParams::getTailLength(int type)
    {
        if (type == ProtocolDisplayIndex::TCP)
        {
            return this->tcp_tail_length;
        }
        else if (type == ProtocolDisplayIndex::UDP)
        {
            return this->udp_tail_length;
        }
        else if (type == ProtocolDisplayIndex::LOCAL)
        {
            return this->local_tail_length;
        }
        else
        {
            return 0;
        }
    }

    int NetToRaindropParams::getFallSecondsMultiplier(int type)
    {
        if (type == ProtocolDisplayIndex::TCP)
        {
            return this->tcp_fall_seconds_multiplier;
        }
        else if (type == ProtocolDisplayIndex::UDP)
        {
            return this->udp_fall_seconds_multiplier;
        }
        else if (type == ProtocolDisplayIndex::LOCAL)
        {
            return this->local_fall_seconds_multiplier;
        }
        else
        {
            return 0;
        }
    }
}
