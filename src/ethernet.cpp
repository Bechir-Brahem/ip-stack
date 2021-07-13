#include "ethernet.h"
void print_eth_hdr(struct eth_hdr *hdr)
{

    char tmp[4];
    std::string dest = "";
    std::string src = "";
    std::string aux;
    dest = mac_from_arr(hdr->dmac);
    src = mac_from_arr(hdr->smac);

    std::cout << "dest: " + dest + " src: " + src << std::endl;
    std::cout << "type: ";
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        std::cout << ("ARP");
        break;
    case ETH_P_IP:
        std::cout << ("IPv4");
        break;
    case ETH_P_IPV6:
        std::cout << ("IPv6");
        break;
    default:
        std::cout << ("unknown frame type");
    }
    std::cout << std::endl;
}

void handle_ether_hdr(struct eth_hdr *hdr)
{
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        handle_ARP(hdr);
        break;
    default:
        return;
    }
}
