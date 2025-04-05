#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
char bfr[25]={0};
char b1[1024]={0};
void main()
{
    int s,ns,rs;
    struct sockaddr_in saddr,caddr,raddr;
    socklen_t l=sizeof(caddr);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(2002);
    saddr.sin_addr.s_addr=INADDR_ANY;
    s=socket(AF_INET,SOCK_STREAM,0);
    if(s<0)
    {
        printf("sock err");
        exit(1);
    }
    printf("sock suc\n");
    if(bind(s,(struct sockaddr *)&saddr,sizeof(saddr))<0)
    {
        printf("bind err");
        exit(1);
    }
    printf("bind suc\n");
    listen(s,5);
    printf("listening on 2002\n");
    ns=accept(s,(struct sockaddr *)&caddr,&l);
   int r= recv(ns,bfr,sizeof(bfr),0);
   bfr[r]='\0';
   printf("hstnm is %s\n",bfr);
   struct hostent *host=gethostbyname(bfr);
   if(host==NULL)
   {
    printf("host err");
        exit(1);
   }
   
   rs=socket(AF_INET,SOCK_STREAM,0);
   if(rs<0)
    {
        printf("rsock err");
        exit(1);
    }
    printf("rsock suc\n");
   raddr.sin_family=AF_INET;
    raddr.sin_port=htons(80);
    raddr.sin_addr=*((struct in_addr *)host->h_addr);
    if(connect(rs,(struct sockaddr *)&raddr,sizeof(raddr))<0)
    {
        printf("connect re err");
        exit(1);
    }
    snprintf(b1,sizeof(b1),"GET / HTTP/1.0\r\nHOST: %s\r\n\r\n",bfr);
    send(rs,b1,strlen(b1),0);
    int n;
while ((n = recv(rs, b1, sizeof(b1) , 0)) > 0) {
    b1[n] = '\0';
    send(ns,b1,strlen(b1),0);
    printf("%s", b1);
}
   close(rs);
   close(ns);
    close(s);
}
