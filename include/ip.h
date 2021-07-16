//
// Created by bb on 7/14/21.
//

#ifndef TUNTAP_IP_H
#define TUNTAP_IP_H

#include "utils.h"
#include <iostream>
#include "icmp.h"
#include <arpa/inet.h>
#include <cstring>
#include <ethernet.h>

struct ip_hdr {
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags_and_offset;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    unsigned char saddr[4];
    unsigned char daddr[4];
    unsigned char payload[];
} __attribute__((packed));

void handle_ip(struct ip_hdr*);
void print_ip(struct ip_hdr* ip);
void send_ip_packet(size_t payload_size,uint8_t proto,const std::string& saddr,const std::string& daddr,unsigned char payload[]);
uint16_t checksum(uint16_t* ptr,size_t size);
#endif //TUNTAP_IP_H
