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
    //checking checksum
    uint16_t  tmp_csum=ip->csum;
    ip->csum=0;

    if(tmp_csum!=checksum((uint16_t*)ip,ip->ihl*2))
    {
        cout<<"invalid checksum\n";
        return ;
    }
    ip->csum=tmp_csum;
    //TODO:check fragmentation
    switch(ip->proto)
    {
        //ICMP
        case IPPROTO_ICMP:
            handle_icmp((struct icmp_hdr*)ip->payload,ntohs(ip->len)-ip->ihl*4);
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
    packet->len= htons(static_cast<uint16_t>(size));
    packet->id=0x5;
    packet->flags_and_offset=64;
    packet->ttl=64;
    packet->proto=proto;
    packet->csum=0;
    parse_ip(packet->saddr,saddr);
    //TODO fix ip
    parse_ip(packet->daddr,daddr);
    memcpy(packet->payload,payload,payload_size);

    packet->csum= checksum((uint16_t *)packet,packet->ihl*2);
    //TODO fix mac with arp request
    unsigned char mac[6];
    parse_mac(mac,MY_HWADDR);
    auto eth_frame= create_ethernet_hdr(mac,mac,(unsigned char*)packet,size,ETH_P_IP);
    send_frame(eth_frame,size);
    free(eth_frame);
    free(packet);
    cout<<"ip packet sent"<<endl;




}

uint16_t checksum(uint16_t* ptr,size_t size){
    uint32_t ret=0;
    uint32_t tmp=1<<16;
    while(size){
        ret+=*ptr;
        ptr++;
        size--;
        if((ret & tmp) !=0)
        {
            ret-=tmp;
            ret++;
        }
    }
    return (uint16_t )~ret;
}
