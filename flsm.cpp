#include <iostream>
#include "iputils.hpp"

int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);

    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " (IP address) (CIDR mask) (NumberOfSubnets)" << std::endl;
        return 1;
    }

    uint32_t ipBitMask;
    uint8_t cidr = std::atoi(argv[2]);
    uint8_t subnetBitNum;
    uint32_t numSubnet = std::atoi(argv[3]);
    try
    {
        ipBitMask = IPUtils::IPBitmaskFromString(argv[1]);

        if (!IPUtils::checkIfNet(ipBitMask, cidr))
            throw IPUtils::E_IP_NOT_NET;

        subnetBitNum = fastCeilLog2(numSubnet);
        if (!IPUtils::checkMask(cidr + subnetBitNum))
            throw IPUtils::E_INVALID_SUBNET;
    }
    catch (uint8_t e)
    {
        switch (e)
        {
            case IPUtils::E_INVALID_OCTECTS:
                std::cout << "Invalid IP address: number of octects is not four." << std::endl;
                break;
            case IPUtils::E_INVALID_OCT_VAL:   
                std::cout << "Invalid IP address: invalid value for octect (0-255)." << std::endl;
                break;
            case IPUtils::E_INVALID_CIDR_VAL:
                std::cout << "Invalid mask: CIDR out of range (1-29)." << std::endl;
                break;
            case IPUtils::E_IP_NOT_NET:
                std::cout << "Invalid IP address: not a net IP." << std::endl;
                break;
            case IPUtils::E_INVALID_SUBNET:
                std::cout << "Invalid number of subnets: resulting subnet mask is invalid." << std::endl;
                break;
        }
        return 1;
    }

    uint8_t submask = cidr + subnetBitNum;

    std::cout << "Name\t\t\tSubnet\t\t\tBroadcast\tGateway" << std::endl;
    for (uint32_t i = 0; i < numSubnet; i++)
    {
        uint32_t currentSub = ipBitMask + (i << (32 - submask));

        std::cout << "Subnet " << i << "\t\t";
        IPUtils::printNetDetails(currentSub, submask);
        std::cout << std::endl;
    }

    return 0;
}