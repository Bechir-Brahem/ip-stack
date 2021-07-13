//
// Created by bb on 7/13/21.
//

#ifndef TUNTAP_UTILS_H
#define TUNTAP_UTILS_H

#include<iostream>
const std::string MY_HWADDR = "da:e8:0d:67:e7:fd";
const std::string MY_IP = "10.0.0.2";
const std::string MAC_BROADCAST = "ff:ff:ff:ff:ff:ff";
const std::string DEV_NAME="tap1";
static int tun_fd;

void parse_mac(unsigned char* bytes, std::string const& in) ;
void parse_ip(unsigned char* bytes, std::string const& in) ;
std::string mac_from_arr(unsigned char *t);
std::string ip_from_arr(unsigned char *t);
#endif //TUNTAP_UTILS_H
