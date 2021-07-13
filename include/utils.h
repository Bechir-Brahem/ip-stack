#ifndef UTILS_H
#define UTILS_H

#include<iostream>

void parse_mac(unsigned char* bytes, std::string const& in) ;
void parse_ip(unsigned char* bytes, std::string const& in) ;
std::string mac_from_arr(unsigned char *t);
std::string ip_from_arr(unsigned char *t);

#endif


