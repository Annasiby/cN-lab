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
    int s,ns;
    struct sockaddr_in saddr,caddr;
    socklen_t l=sizeof(caddr);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(2002);
    saddr.sin_addr.s_addr=INADDR_ANY;
    s=socket(PF_INET,SOCK_STREAM,0);
    if(s<0)
    {
        printf("err\n");
        exit(1);
    }
    printf("socket\n");
    if(bind(s,(struct sockaddr *)&saddr,l)<0)
    {
        printf("err\n");
        exit(1);
    }
    printf("bind\n");
    listen(s,5);
    ns=accept(s,(struct sockaddr *)&caddr,&l);
    int k=5,m=1;
    while(k!=0)
    {
        int r=recv(ns,b2,sizeof(b2),0);
        if(strncmp(b2,"frame",5)==0)
        {
            printf("frame %d received\n",m);
        }
        else{
          
            printf("not received\n");
           
        }
       
        if(m%2==0)
        {
            strcpy(b1,"ack");
            
        }
        else{
            strcpy(b1,"kca");
            printf("ack lost\n");
            for(int i=1;i<=3;i++)
            {
                printf("waiting sec %d\n",i);
                    
                sleep(1);
            }
            printf("retransmitting ack\n");
            strcpy(b1,"ack");
        }
        send(ns,b1,strlen(b1),0);
        
        k--;
        m++;
    }
    close(ns);
    close(s);
}
