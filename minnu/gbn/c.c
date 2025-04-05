#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
char b1[1024]={0};
char b2[1024]={0};
void main()
{
    int c;
    struct sockaddr_in saddr;
    socklen_t l=sizeof(saddr);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(2002);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    c=socket(PF_INET,SOCK_STREAM,0);
    if(c<0)
    {
        printf("err\n");
        exit(1);
    }
    printf("socket\n");
    if(connect(c,(struct sockaddr *)&saddr,l)<0)
    {
        printf("err\n");
        exit(1);
    }
    printf("connect\n");
    int k=5,m=1;
    while(k!=0)
    {
        if(m<=5)
        {
            printf("sending\n");
        }
        if(m%2==0)
        {
            strcpy(b1,"frame");
            printf("packet %d sent",m);
        }
        else{
            strcpy(b1,"error");
            printf("pkt lost\n");
            for(int i=1;i<=3;i++)
            {
                printf("waiting sec %d\n",i);
                    
                sleep(1);
            }
            printf("retransmitting packet\n");
            strcpy(b1,"frame");
        }
        send(c,b1,strlen(b1),0);
        int r=recv(c,b2,sizeof(b2),0);
        if(strncmp(b2,"ack",3)==0)
        {
            printf("ack for %d\n",m);
        }
        else{
            printf(" no ack for %d\n",m);
        }
        k--;
        m++;
    }
    close(c);
}
