#pragma once

namespace N_CodeRain
{
    enum ProtocolDisplayIndex
    {
        TCP = 0,
        UDP = 1,
        LOCAL = 2,
        Protocols = 3  // Number of used protocols (to be used in loops)
    };

    enum NetworkAvailabilityIndex
    {
        Network_Unavailable = 0,
        Network_Available = 1,
        OptionCount = 2  // Number of network availability options (to be used in loops)
    };
}
