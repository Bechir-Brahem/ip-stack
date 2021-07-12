#include <arpa/inet.h>
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

#define ETH_TYPE_ARP 1544

using namespace std;

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
    case ETH_TYPE_ARP:
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
void handle_ARP(struct eth_hdr* hdr)
{
    struct arp_hdr *arp = (struct arp_hdr *)hdr->payload;
    struct arp_ip_hdr *arp_ip=(struct arp_ip_hdr*)arp->data;


    cout << "=====================================" << endl;
    cout << "          arp header" << endl;
    cout << "=====================================" << endl;

    cout << "hardware address type " << arp->hrd << endl;
    cout << "protocol type " << arp->pro << endl;
    cout << "hardware address size " << arp->hln << endl;
    cout << "protocol address " << arp->pln << endl;
    cout << "ARP op code " << arp->op << endl;
    cout<<"====================================="<<endl;
    cout<<"sender mac address "<<mac_from_arr(arp_ip->smac) <<endl;
    cout<<"sender ip address "<<ip_from_arr(arp_ip->sip)<<endl;
    cout<<"target mac address "<<mac_from_arr(arp_ip->dmac)<<endl;
    cout<<"target ip address "<<ip_from_arr(arp_ip->dip)<<endl;
    cout << "=====================================" << endl;
    cout << endl;
}
void check_ethertype(struct eth_hdr *hdr)
{
    switch (hdr->ethertype) {
    case ETH_TYPE_ARP:
        handle_ARP(hdr);
        break;
    default:
        return;
    }
}
int main()
{
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
        struct eth_hdr *hdr = (struct eth_hdr *)buffer;
        print_eth_hdr(hdr);
        check_ethertype(hdr);

        /* Do whatever with the data */
        printf("Read %d bytes from device %s\n", nread, tun_name);
    }
}
