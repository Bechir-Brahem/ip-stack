#include "arp.h"
using namespace std;
/**
 * @brief prints an arp header
 *
 * @param arp
 * @param arp_ip
 */
void print_arp(struct arp_hdr *arp, struct arp_ip_hdr *arp_ip)
{
    cout << "=====================================" << '\n';
    cout << "          arp header" << '\n';
    cout << "=====================================" << '\n';

    cout << "hardware address type " << hex << arp->hrd <<dec<< '\n';
    cout << "protocol type " << hex << arp->pro <<dec<< '\n';
    cout << "hardware address size " << (int)arp->hln << '\n';
    cout << "protocol address " << (int)arp->pln << '\n';
    cout << "ARP op code " << arp->op << '\n';
    cout << "=====================================" << '\n';
    cout << "sender mac address " << mac_from_arr(arp_ip->smac) << '\n';
    cout << "sender ip address " << ip_from_arr(arp_ip->sip) << '\n';
    cout << "target mac address " << mac_from_arr(arp_ip->dmac) << '\n';
    cout << "target ip address " << ip_from_arr(arp_ip->dip) << '\n';
    cout << "=====================================" << '\n';
    cout << endl;
}


/*
 * transmission layer (not necessarily accessible to
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
arp_hdr *create_arp_hdr(uint16_t opcode, unsigned char *smac,
                        unsigned char *sip, unsigned char *dmac,
                        unsigned char *dip)
{
    auto arp_ip = (struct arp_ip_hdr *)malloc(sizeof(struct arp_ip_hdr));
    memcpy(arp_ip->smac, smac, 6 * sizeof(unsigned char));
    memcpy(arp_ip->dmac, dmac, 6 * sizeof(unsigned char));
    memcpy(arp_ip->sip, sip, 4 * sizeof(unsigned char));
    memcpy(arp_ip->dip, dip, 4 * sizeof(unsigned char));

    auto arp = (struct arp_hdr *)malloc(sizeof(struct arp_hdr)+sizeof(struct arp_ip_hdr));
    arp->hrd = htons(0x0001);
    arp->pro = htons(0x0800);
    arp->op = htons(opcode);
    arp->hln = 6;
    arp->pln=4;
    memcpy(arp->data, arp_ip, sizeof(arp_ip_hdr));
    free(arp_ip);
    return arp;
}
void handle_ARP(struct eth_hdr *hdr)
{
    auto *arp = (struct arp_hdr *)hdr->payload;
     arp->hrd = ntohs(arp->hrd);
     arp->pro = ntohs(arp->pro);
     arp->op = ntohs(arp->op);
    auto *arp_ip = (struct arp_ip_hdr *)arp->data;
    print_arp(arp, arp_ip);
    // check hardware address space must be ethernet
     if (arp->hrd != 0x0001 and arp->hln != 6)
         return;
     // check protocol
     // i accept IPv4 only
     if (arp->pro != 0x0800 and arp->pro != 4)
         return;
     /**
      * TODO: add to translation table
      */

     /**
      * @brief check if i am the destination
      */
    const string dmac = mac_from_arr(arp_ip->dmac);
     if (MY_HWADDR != dmac and MAC_BROADCAST != dmac and dmac!="00:00:00:00:00:00")
         return;

    if (arp->op != 1)
         return;

    unsigned char my_mac[6];
    unsigned char my_ip[4];
    parse_ip(my_ip, MY_IP);
    parse_mac(my_mac, MY_HWADDR);
    auto reply=create_arp_hdr((uint16_t)2, my_mac, my_ip, arp_ip->smac, arp_ip->sip);
    print_arp(reply,(struct arp_ip_hdr*)reply->data);

    size_t payload_size=sizeof(struct arp_ip_hdr) + sizeof(struct arp_hdr);
auto eth=create_ethernet_hdr(arp_ip->smac, my_mac,(unsigned char *)(reply),payload_size );
    send_frame(eth,payload_size);
    free(eth);
    free(reply);

}
