#include <iostream>
using namespace std;
#include "utils.h"
#include "tuntap.h"
#include "ethernet.h"


const string MY_HWADDR = "da:e8:0d:67:e7:fd";
const string MY_IP = "10.0.0.2";

int main()
{
    unsigned char mac[6];
    unsigned char ip[4];
    parse_mac(mac, MY_HWADDR);
    parse_ip(ip, MY_IP);
    cout<<mac_from_arr(mac)<<endl;;
    cout<<ip_from_arr(ip)<<endl;
    char tun_name[IFNAMSIZ];
    char buffer[500];

    /* Connect to the device */
    strcpy(tun_name, "tap1");
    int tun_fd = tun_alloc(tun_name, IFF_TAP | IFF_NO_PI); /* tun interface */

    if (tun_fd < 0) {
        perror("Allocating interface");
        exit(1);
    }

    /* Now read data coming from the kernel */
    while (1) {
        /* Note that "buffer" should be at least the MTU size of the interface,
         * eg 1500 bytes */
        int nread = read(tun_fd, buffer, sizeof(buffer));
        if (nread < 0) {
            perror("Reading from interface");
            close(tun_fd);
            exit(1);
        }
        printf("Read %d bytes from device %s\n", nread, tun_name);
        struct eth_hdr *hdr = (struct eth_hdr *)buffer;
        hdr->ethertype = ntohs(hdr->ethertype);
        print_eth_hdr(hdr);
        handle_ether_hdr(hdr);
    }
}
