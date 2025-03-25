#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h> //
#include<unistd.h>
void main()
{
    int client;
    struct sockaddr_in servaddr;
    char bfr[1024]={0};
    client=socket(AF_INET,SOCK_STREAM,0);
        if(client<0) //not ==
            {
                printf("error");
                exit(EXIT_FAILURE);
            }
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(6265);//integer
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1"); //syntax
    connect(client,(struct sockaddr *)&servaddr,sizeof(servaddr));//<0
 
    while(1){
           memset(bfr,0,sizeof(bfr));
           printf("client: ");
    fgets(bfr,sizeof(bfr),stdin);
    send(client,bfr,sizeof(bfr),0);
    if(strcmp(bfr,"bye")==0)
    {
        //recv(client,bfr,sizeof(bfr),0);
        break;
    }
     memset(bfr,0,sizeof(bfr));
     recv(client,bfr,sizeof(bfr),0);
     printf("server: %s",bfr);
    }
    close(client);
}
