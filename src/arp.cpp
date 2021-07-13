#include "arp.h"
using namespace std;
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