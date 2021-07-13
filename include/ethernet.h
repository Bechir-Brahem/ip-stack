#ifndef ETHERNET_H
#define ETHERNET_H

#include <iostream>
#include <cstdint>
#include <linux/if_ether.h>
#include <type_traits>
#include "utils.h"
#include "arp.h"

struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

void print_eth_hdr(struct eth_hdr *hdr);
void handle_ether_hdr(struct eth_hdr *hdr);


#endif
