#include <iostream>
#include <algorithm>
#include <vector>
#include "iputils.hpp"

void traverseSubnet(uint32_t ip, uint8_t mask, std::vector<int>* subs)
{
    // Our job here is done
    if (subs->size() == 0)
        return;

    int hostBitsRequired = fastCeilLog2(subs->at(0) + 2);
    
    // If the current sub isn't perfectly sized, keep dividing
    if (32 - mask > hostBitsRequired)
    {
        traverseSubnet(ip + (1 << (32 - mask - 1)), mask + 1, subs);
        traverseSubnet(ip, mask + 1, subs);
        return;
    }

    // A suitable subnet was found, print subnet details and return to caller
    std::cout << "Subnet with " << subs->at(0) << " hosts \t";
    IPUtils::printNetDetails(ip, mask);
    std::cout << "\t" << subs->at(0) + 2 << "/" << (1 << hostBitsRequired) << std::endl;
    subs->erase(subs->begin());
}

int main(int argc, char** argv)
{
    std::ios::sync_with_stdio(false);

    if (argc < 4)
    {
        std::cout << "Usage: " << argv[0] << " (IP address) (CIDR mask) [Subnets...]" << std::endl;
        return 1;
    }

    uint32_t tot = 0;
    std::vector<int> subnets = std::vector<int>(argc - 3);

    for (int i = 0; i < argc - 3; i++)
    {
        subnets.at(i) = std::atoi(argv[i + 3]);
        tot += (1 << fastCeilLog2(subnets.at(i) + 2));
    }

    std::sort(subnets.begin(), subnets.end(), std::greater<int>());

    std::string ip = argv[1];
    uint32_t ipBitMask;
    uint8_t cidr = std::atoi(argv[2]);

    // Format IP address
    try
    {
        ipBitMask = IPUtils::IPBitmaskFromString(ip);

        if (!IPUtils::checkIfNet(ipBitMask, cidr))
            throw IPUtils::E_IP_NOT_NET;

        if (1 << (32 - cidr) < tot)
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
                std::cout << "Invalid number of subnets: can't fit input subnets in given range." << std::endl;
                break;
        }
        return 1;
    }

    std::cout << "Name\t\t\tSubnet\t\t\tBroadcast\tGateway\t\tUsage" << std::endl;
    traverseSubnet(ipBitMask, cidr, &subnets);

    return 0;
}