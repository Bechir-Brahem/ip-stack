#include<iostream>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
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
    char *clonedev = "/dev/net/tun";
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
using namespace std;
void print_eth_hdr(struct eth_hdr *hdr)
{
    char dest[30];
    char source[15];
    dest[0] = '\0';
    char tmp[4];
    int i;
/*
 *     for (i = 0; i < 5; i++) {
 *         sprintf(tmp, "%02x:", hdr->dmac[i]);
 *         strcat(dest, tmp);
 *     }
 *     sprintf(tmp, "%02x", hdr->dmac[i]);
 *     strcat(dest, tmp);
 *     strcat(source, "\0");
 * 
 *     
 *     for (i = 0; i < 5; i++) {
 *         sprintf(tmp, "%02x:", hdr->smac[i]);
 *         strcat(source, tmp);
 *     }
 *     sprintf(tmp, "%02x", hdr->smac[i]);
 *     strcat(source, tmp);
 *     strcat(source, "\0");
 */

    string dests="";
    string aux;
    for(i=0;i<6;i++)
    {
        sprintf(tmp,"%02x",hdr->dmac[i]);
        aux=string(tmp);
        dests+=aux;
        dests+=':';
    }
    dests.pop_back();
    cout<<dests;
   // printf("destination: %s  from: %s\n", dest,source);
    // printf("type: ");
/*
 * 
 * 
 *     switch(hdr->ethertype)
 *     {
 *         case ETH_P_ARP:
 *             printf("ARP");
 *             break;
 *         case ETH_P_IP:
 *             printf("IPv4");
 *             break;
 *         case ETH_P_IPV6:
 *             printf("IPv6");
 *             break;
 *             default:
 *             printf("unknown frame type");
 *     }
 */
    printf("\n");

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

        /* Do whatever with the data */
        printf("Read %d bytes from device %s\n", nread, tun_name);
    }
}
