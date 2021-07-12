#include <arpa/inet.h>
#include <sstream>
#include <cstdio>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <linux/if_ether.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

#define ETH_TYPE_ARP 1544
const string MY_HWADDR = "da:e8:0d:67:e7:fd";
const string MY_IP = "10.0.0.2";


struct eth_hdr {
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t ethertype;
    unsigned char payload[];
} __attribute__((packed));

int tun_alloc(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = (char *)"/dev/net/tun";
    /* Arguments taken by the function:
     *
     * char *dev: the name of an interface (or '\0'). MUST have enough
     *   space to hold the interface name if '\0' is passed
     * int flags: interface flags (eg, IFF_TUN etc.)
     */
    /* open the clone device */
    if ((fd = open(clonedev, O_RDWR)) < 0) { return fd; }
    /* preparation of the struct ifr, of type "struct ifreq" */
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags; /* IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI */

    if (*dev) {
        /* if a device name was specified, put it in the structure; otherwise,
         * the kernel will try to allocate the "next" device of the
         * specified type */
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    /* try to create the device */
    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        close(fd);
        return err;
    }

    /* if the operation was successful, write back the name of the
     * interface to the variable "dev", so the caller can know
     * it. Note that the caller MUST reserve space in *dev (see calling
     * code below) */
    strcpy(dev, ifr.ifr_name);

    /* this is the special file descriptor that the caller will use to talk
     * with the virtual interface */
    return fd;
}
/*
 * converts string mac (ff:da:23...)-> to char[6]
 */
/*
 * void mac_to_arr(unsigned char *mac, string adr)
 * {
 *     string tmp;
 *     for (int i = 0; i < 18; i += 3) {
 *         tmp="";
 *         tmp+=adr[i];
 *         tmp+=adr[i+1];
 *         unsigned int x;
 *         stringstream ss;
 *         ss << std::hex << tmp;
 *         ss >> x;
 *         mac[i/3]=x;
 *     }
 * }
 * 
 */
void parse_mac(unsigned char* bytes, std::string const& in) {
    if (std::sscanf(in.c_str(),
                    "%02x:%02x:%02x:%02x:%02x:%02x",
                    bytes, bytes+1, bytes+2,
                    bytes+3, bytes+4, bytes+5) != 6)
    {
        throw std::runtime_error(in+std::string(" is an invalid MAC address"));
    }
}
void parse_ip(unsigned char* bytes, std::string const& in) {
    if (std::sscanf(in.c_str(),
                    "%d.%d.%d.%d",
                    bytes, bytes+1, bytes+2,
                    bytes+3) != 4)
    {
        throw std::runtime_error(in+std::string(" is an invalid IP address"));
    }
}
// void ip_to_arr(unsigned char *ip,string adr)
// {

    // string tmp;
    // for (int i = 0; i < 18; i += 3) {
        // tmp="";
        // tmp+=adr[i];
        // tmp+=adr[i+1];
        // unsigned int x;
        // stringstream ss;
        // ss << std::hex << tmp;
        // ss >> x;
        // ip
    // }

// }
/*
 * converts array of unsigned char [6] to a printable string of mac address
 */
string mac_from_arr(unsigned char *t)
{
    string dest = "";
    char tmp[4];
    string aux;
    for (int i = 0; i < 6; i++) {
        sprintf(tmp, "%02x", t[i]);
        aux = string(tmp);
        dest += aux;
        dest += ':';
    }
    dest.pop_back();
    return dest;
}
/*
 * converts array of unsigned char [4] to printable string ip dotted format
 */
string ip_from_arr(unsigned char *t)
{
    string dest = "";
    char tmp[4];
    string aux;
    for (int i = 0; i < 4; i++) {
        sprintf(tmp, "%d", t[i]);
        aux = string(tmp);
        dest += aux;
        dest += '.';
    }
    dest.pop_back();
    return dest;
}
void print_eth_hdr(struct eth_hdr *hdr)
{

    char tmp[4];
    string dest = "";
    string src = "";
    string aux;
    dest = mac_from_arr(hdr->dmac);
    src = mac_from_arr(hdr->smac);

    cout << "dest: " + dest + " src: " + src << endl;
    cout << "type: ";
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        cout << ("ARP");
        break;
    case ETH_P_IP:
        cout << ("IPv4");
        break;
    case ETH_P_IPV6:
        cout << ("IPv6");
        break;
    default:
        cout << ("unknown frame type");
    }
    cout << endl;
}
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

void print_arp(struct arp_hdr *arp, struct arp_ip_hdr *arp_ip)
{
    cout << "=====================================" << endl;
    cout << "          arp header" << endl;
    cout << "=====================================" << endl;

    cout << "hardware address type " << arp->hrd << endl;
    cout << "protocol type " << arp->pro << endl;
    cout << "hardware address size " << (int)arp->hln << endl;
    cout << "protocol address " << (int)arp->pln << endl;
    cout << "ARP op code " << arp->op << endl;
    cout << "=====================================" << endl;
    cout << "sender mac address " << mac_from_arr(arp_ip->smac) << endl;
    cout << "sender ip address " << ip_from_arr(arp_ip->sip) << endl;
    cout << "target mac address " << mac_from_arr(arp_ip->dmac) << endl;
    cout << "target ip address " << ip_from_arr(arp_ip->dip) << endl;
    cout << "=====================================" << endl;
    cout << endl;
}
/*
 * Ethernet transmission layer (not necessarily accessible to
 *         the user):
 *        48.bit: Ethernet address of destination
 *        48.bit: Ethernet address of sender
 *        16.bit: Protocol type = ether_type$ADDRESS_RESOLUTION
 *    Ethernet packet data:
 *        16.bit: (ar$hrd) Hardware address space (e.g., Ethernet,
 *                         Packet Radio Net.)
 *        16.bit: (ar$pro) Protocol address space.  For Ethernet
 *                         hardware, this is from the set of type
 *                         fields ether_typ$<protocol>.
 *         8.bit: (ar$hln) byte length of each hardware address
 *         8.bit: (ar$pln) byte length of each protocol address
 *        16.bit: (ar$op)  opcode (ares_op$REQUEST | ares_op$REPLY)
 *        nbytes: (ar$sha) Hardware address of sender of this
 *                         packet, n from the ar$hln field.
 *        mbytes: (ar$spa) Protocol address of sender of this
 *                         packet, m from the ar$pln field.
 *        nbytes: (ar$tha) Hardware address of target of this
 *                         packet (if known).
 *        mbytes: (ar$tpa) Protocol address of target.
 */
void handle_ARP(struct eth_hdr *hdr)
{
    struct arp_hdr *arp = (struct arp_hdr *)hdr->payload;
    arp->hrd = ntohs(arp->hrd);
    arp->pro = ntohs(arp->pro);
    struct arp_ip_hdr *arp_ip = (struct arp_ip_hdr *)arp->data;
    print_arp(arp, arp_ip);
    // check hardware address space
    if (arp->hrd != 0x0001) return;
    // check protocol
    // i accept IPv4 only
    if (arp->pro != 0x0800) return;
}
void check_ethertype(struct eth_hdr *hdr)
{
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        handle_ARP(hdr);
        break;
    default:
        return;
    }
}
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
        check_ethertype(hdr);
    }
}
