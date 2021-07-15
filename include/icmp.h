//
// Created by bb on 7/15/21.
//

#ifndef TUNTAP_ICMP_H
#define TUNTAP_ICMP_H

#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include "ip.h"

const uint8_t ICMP_ECHO_REQ=8;
const uint8_t ICMP_ECHO_REP=0;
const uint8_t ICMP_DST=3;

struct icmp_hdr{
    uint8_t type;
    uint8_t code;
    uint16_t csum;
    uint8_t data[];
} __attribute__((packed));

struct icmp_echo{
    uint16_t id;
    uint16_t seq;
    uint8_t data[];
} __attribute__((packed));

void handle_icmp(struct icmp_hdr*,int);
void handle_icmp_echo(struct icmp_echo* icmp,int);
void print_icmp_echo(icmp_echo *icmp) ;


#endif //TUNTAP_ICMP_H
