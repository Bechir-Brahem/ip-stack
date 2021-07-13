#ifndef ARP_H
#define ARP_H

#include <netinet/in.h>
#include "ethernet.h"
#include<cstring>
struct arp_hdr {
    uint16_t hrd;
    uint16_t pro;
    unsigned char hln;
    unsigned char pln;
    uint16_t op;
    unsigned char data[];
} __attribute__((packed));

struct arp_ip_hdr {
    unsigned char smac[6];
    unsigned char sip[4];
    unsigned char dmac[6];
    unsigned char dip[4];
} __attribute__((packed));



void print_arp(struct arp_hdr *arp, struct arp_ip_hdr *arp_ip);
void handle_ARP(struct eth_hdr *hdr);

#endif
