#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
char bfr[1024]={0};
char b1[1024]={0};
void main()
{
    int c;
    struct sockaddr_in saddr;
    socklen_t l=sizeof(saddr);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(2002);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    c=socket(AF_INET,SOCK_STREAM,0);
    if(c<0)
    {
        printf("sock err");
        exit(1);
    }
    printf("sock suc\n");
    if(connect(c,(struct sockaddr *)&saddr,sizeof(saddr))<0)
    {
        printf("connect err");
        exit(1);
    }
    printf("connect suc\n");
    fgets(bfr,sizeof(bfr),stdin);
    bfr[strcspn(bfr,"\n")]='\0';
    send(c,bfr,strlen(bfr),0);
   
    int r;
    while((r=recv(c,b1,sizeof(b1),0))>0)
    {
        b1[r]='\0';
        printf("%s",b1);
    }
    close(c);
}
