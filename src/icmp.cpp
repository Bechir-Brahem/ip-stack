//
// Created by bb on 7/15/21.
//
#include"icmp.h"


using namespace std;
void print_icmp(struct icmp_hdr* icmp)
{
    cout<<"*************ICMP***************\n";
    cout<<"type: "<<(int)icmp->type<<'\n';
    cout<<"code: "<<(int)icmp->code<<'\n';
    cout<<"csum: "<<hex<<ntohs(icmp->csum)<<dec<<endl;
}
void handle_icmp(struct icmp_hdr* icmp,int icmp_size)
{
    print_icmp(icmp);
    if(icmp->type==ICMP_ECHO_REQ)
    {
        print_icmp_echo((struct icmp_echo*) icmp->data);

        auto reply =(struct icmp_hdr*) malloc((size_t)icmp_size);
        reply->type=ICMP_ECHO_REP;
        reply->code=0;
        reply->csum=0;
        memcpy(reply->data, icmp->data, (unsigned long)(icmp_size) - sizeof(struct icmp_hdr));
        reply->csum= checksum((uint16_t*)reply,(size_t)icmp_size/2);
        //TODO fix ip problem
        printf("ICMP SIZE %d",icmp_size);
        send_ip_packet((size_t)(icmp_size), IPPROTO_ICMP, MY_IP, "10.0.0.2", (unsigned char*)reply);
        free(reply);
        cout<<"ICMP packet sent"<<endl;


    }


}


void print_icmp_echo(icmp_echo *icmp) {
    cout<<"####### PING ########\n";
    cout<<"id: "<<icmp->id;
    cout<<"\nseq: "<<icmp->seq<<'\n'<<endl;
}
