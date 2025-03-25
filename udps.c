#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
void main()
{
    //here 1 int and no newsock like tcps as no accept and listen here, 2 address like tcpserver, sizeof(2) like tcps, 2 msg like tcps
    int s;
    struct sockaddr_in saddr,caddr;
    char cmsg[2000],smsg[2000];
    int cl=sizeof(caddr);
    s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(s<0)
    {
        printf("err");
        exit(1);
    }
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    saddr.sin_port=htons(2002);
    saddr.sin_family=AF_INET;
    if(bind(s,(struct sockaddr *)&saddr,sizeof(saddr))<0)
     {
        printf("err");
        exit(1);
    }
    recvfrom(s,cmsg,sizeof(cmsg),0,(struct sockaddr *)&caddr,&cl); // &cl
    printf("%s",cmsg);
    strcpy(smsg,"server\n");
     sendto(s,smsg,strlen(smsg),0,(struct sockaddr *)&caddr,sizeof(saddr));
    close(s);
    //in udp both are client addresses for sendto and receive from
}
