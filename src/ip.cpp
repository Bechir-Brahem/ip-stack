//
// Created by bb on 7/14/21.
//

#include "ip.h"

using namespace std;

void print_ip(struct ip_hdr* ip)
{
    cout<<"*** IPv4 ***"<<'\n';
    cout<<"version: "<<hex<<(int) ip->version<<dec<<'\n';
    cout<<"ihl: "<<(int) ip->ihl<<'\n';
    cout<<"tos: "<<(int) ip->tos<<'\n';
    cout<<"len: "<<(int) ntohs(ip->len)<<'\n';
    cout<<"id: "<<hex<<(int) ntohs(ip->id)<<dec<<'\n';
    cout<<"flag + frag_offset(not used): "<<(int)ip->flags_and_offset<<'\n';//byte order problem
    cout<<"TTL: "<<(int) ip->ttl<<'\n';
    if((int)ip->proto==IPPROTO_ICMP)
    cout<<"proto: ICMP\n";
    else
        cout<<"proto: "<<(int) ip->proto<<'\n';
    cout<<"checksum: "<<hex<<(int) ntohs(ip->csum)<<dec<<'\n';
    cout<<"source: "<<ip_from_arr(ip->saddr)<<'\n';
    cout<<"dest: "<<ip_from_arr(ip->daddr)<<'\n'<<endl;


}

void handle_ip(struct ip_hdr* ip)
{
    print_ip(ip);
    if(ip->version!=4){
        cerr<<"ip packet is not ipv4";
        return ;}
    if(ip->ihl<5){
        cerr<<"header length is invalid";
        return;
    }
    if(MY_IP!= ip_from_arr(ip->daddr))
    {
        return;
    }
    //TODO:check fragmentation
    //TODO:check checksum
    switch(ip->proto)
    {
        //ICMP
        case IPPROTO_ICMP:
            handle_icmp((struct icmp_hdr*)ip->payload,ntohs(ip->len)-ip->ihl);
            break;
        case IPPROTO_TCP:
            //handle_TCP();
            break;


    }

}
void send_ip_packet(size_t payload_size,uint8_t proto,const string& saddr,const string& daddr,unsigned char payload[])
{
    size_t size=payload_size+sizeof(struct ip_hdr);
    auto packet=(struct ip_hdr*) malloc(size);
    packet->ihl=5;
    packet->version=4;
    packet->tos=0;
    packet->len= static_cast<uint16_t>(size);
    packet->id=0x5;
    packet->flags_and_offset=64;
    packet->ttl=64;
    packet->proto=proto;
    //TODO fix checksum
    packet->csum=0x11;
    parse_ip(packet->saddr,saddr);
    //TODO fix ip
    parse_ip(packet->daddr,daddr);
    memcpy(packet->payload,payload,payload_size);
    //TODO fix mac with arp request
    unsigned char mac[4];
    parse_mac(mac,MY_HWADDR);
    auto eth_frame= create_ethernet_hdr(mac,mac,(unsigned char*)packet,size,ETH_P_IP);
    send_frame(eth_frame,size);
    cout<<"ip packet sent"<<endl;




}
