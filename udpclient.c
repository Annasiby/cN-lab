#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
void main()  //here client, address of both,2 msg , sento recvfrom with syntax of prev +new, <0 checking all, send - strlen
{
    int client;
    struct sockaddr_in ser;//not socketaddr
    char smsg[2000],cmsg[2000];
    int slen=sizeof(ser);
    client=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(client<0)
    {
        printf("error");
        exit(1);
    }
    //no connect here
    ser.sin_addr.s_addr=inet_addr("127.0.0.1");
    ser.sin_family=AF_INET;
    ser.sin_port=htons(2002);
    fgets(cmsg,sizeof(cmsg),stdin);
    sendto(client,cmsg,strlen(cmsg),0,(struct sockaddr*)&ser,slen);
    recvfrom(client,smsg,sizeof(smsg),0,(struct sockaddr*)&ser,&slen);
    printf("%s",smsg);
    close(client);
}
