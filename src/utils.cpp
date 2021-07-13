#include "utils.h"
/*
 * converts string mac (ff:da:23...)-> to char[6]
 */
void parse_mac(unsigned char *bytes, std::string const &in)
{
    if (std::sscanf(in.c_str(),
                    "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
                    bytes, bytes + 1, bytes + 2,
                    bytes + 3, bytes + 4, bytes + 5) != 6)
    {
        throw std::runtime_error(in + std::string(" is an invalid MAC address"));
    }
}
/*
 * converts string ip (192.168.1.1) to char [4]
 */
void parse_ip(unsigned char *bytes, std::string const &in)
{
    if (std::sscanf(in.c_str(),
                    "%hhu.%hhu.%hhu.%hhu",
                    bytes, bytes + 1, bytes + 2,
                    bytes + 3) != 4)
    {
        throw std::runtime_error(in + std::string(" is an invalid IP address"));
    }
}
/*
 * converts array of unsigned char [6] to a printable string of mac address
 */
std::string mac_from_arr(unsigned char *t)
{
    std::string dest;
    char tmp[4];
    std::string aux;
    for (int i = 0; i < 6; i++)
    {
        sprintf(tmp, "%02x", t[i]);
        aux = std::string(tmp);
        dest += aux;
        dest += ':';
    }
    dest.pop_back();
    return dest;
}
/*
 * converts array of unsigned char [4] to printable string ip dotted format
 */
std::string ip_from_arr(unsigned char *t)
{
    std::string dest;
    char tmp[4];
    std::string aux;
    for (int i = 0; i < 4; i++)
    {
        sprintf(tmp, "%d", t[i]);
        aux = std::string(tmp);
        dest += aux;
        dest += '.';
    }

    dest.pop_back();
    return dest;
}
