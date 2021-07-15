#include "ethernet.h"
using namespace std;
void print_eth_hdr(struct eth_hdr *hdr)
{

    std::string dest;
    std::string src;
    std::string aux;
    dest = mac_from_arr(hdr->dmac);
    src = mac_from_arr(hdr->smac);

    std::cout << "dest: " + dest + " src: " + src << '\n';
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
    std::cout << '\n';
}

void handle_ether_hdr(struct eth_hdr *hdr)
{
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        handle_ARP(hdr);
        break;
    case ETH_P_IP:
        handle_ip((struct ip_hdr*) hdr->payload);
        break;

    default:
        return;
    }
}
struct eth_hdr* create_ethernet_hdr(unsigned char *dmac,unsigned char *smac,const unsigned char payload[],size_t payload_size,uint16_t ethertype)
{

    auto hdr = (struct eth_hdr *)malloc(sizeof(struct eth_hdr)+payload_size);
    memcpy(hdr->dmac, dmac, 6 * sizeof(unsigned char));
    memcpy(hdr->smac, smac, 6 * sizeof(unsigned char));
    hdr->ethertype=htons(ethertype);
    memcpy(hdr->payload, payload,(payload_size));
    return hdr;

}

int send_frame(struct eth_hdr* hdr,size_t payload_size)
{

    unsigned long size=payload_size+sizeof(struct eth_hdr);
    if( (unsigned long) write(tun_fd,hdr,size)!= size)
    {
        perror("error writing frame to device");
        close(tun_fd);
        exit(1);
    }
    std::cout<<"frame sent"<<std::endl;
    return 0;
}
