#ifndef ETHERNET_H
#define ETHERNET_H

#include "iostream"
#include <cstdint>
#include <linux/if_ether.h>
#include <type_traits>
#include "utils.h"
#include "arp.h"
#include "unistd.h"
#include "ip.h"

struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

void print_eth_hdr(struct eth_hdr *hdr);
void handle_ether_hdr(struct eth_hdr *hdr);
int send_frame(struct eth_hdr* hdr,size_t payload_size);
struct eth_hdr* create_ethernet_hdr(unsigned char *dmac,unsigned char *smac,const unsigned char* payload,size_t payload_size,uint16_t ethertype=ETH_P_ARP);


#endif
