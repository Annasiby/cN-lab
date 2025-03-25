/**************minnu*****************
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
void main()
{
    int serv,newsock;
    struct sockaddr_in servaddr,clientaddr; //
    int len=sizeof(clientaddr);//
    char bfr[1024]={0};
    char smsg[1024]={0};
    serv=socket(AF_INET,SOCK_STREAM,0);
    if(serv<0)                              //
    {
        printf("error");
        exit(EXIT_FAILURE);
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(6265);
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    bind(serv,(struct sockaddr *)&servaddr,sizeof(servaddr));
    if(listen(serv,3)<0)
    {
        printf("listen failed");
    }
    newsock=accept(serv,(struct sockaddr *)&clientaddr,&len);//accept not  socket
    while(1)
    {
    memset(bfr,0,sizeof(bfr));
    memset(smsg,0,sizeof(smsg));
    recv(newsock,bfr,sizeof(bfr),0);
    printf("client: %s",bfr);
    if(strcmp(bfr,"hello\n")==0 || strcmp(bfr,"hi\n")==0)
    {
        strcpy(smsg,"hello client!\n");
        send(newsock,smsg,strlen(smsg),0);
    }
    else if(strcmp(bfr,"bye\n")==0)
    {
        strcpy(smsg,"bye!\n");
         send(newsock,smsg,strlen(smsg),0);
         break;
    }
     else if(strcmp(bfr,"how are you\n")==0)
    {
        strcpy(smsg,"fine. you?\n");
       send(newsock,smsg,strlen(smsg),0);
         
    }
    else
    {
         strcpy(smsg,"I am just a simple bot, I dont know about other thing\n");
         send(newsock,smsg,strlen(smsg),0);
    }
    
    }
    printf("chatclosed");
    close(newsock);
    close(serv);
}//in send() use strlen() //in accept() use cliaddr and &its sizeof()
**********************chatgpt****************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6265

int main() {
    int serv, newsock;
    struct sockaddr_in servaddr, clientaddr;
    int len = sizeof(clientaddr);
    char bfr[1024] = {0};
    char smsg[1024] = {0};

    // Create socket
    serv = socket(AF_INET, SOCK_STREAM, 0);
    if (serv < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket
    if (bind(serv, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(serv, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Chatbot is waiting for a client on port %d...\n", PORT);

    // Accept client connection
    newsock = accept(serv, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
    if (newsock < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Client connected! Chat started...\n");

    while (1) {
        // Clear buffers
        memset(bfr, 0, sizeof(bfr));
        memset(smsg, 0, sizeof(smsg));

        // Receive message from client
        recv(newsock, bfr, sizeof(bfr), 0);
        printf("Client: %s", bfr);

        // Check for client messages and respond
        if (strncmp(bfr, "hello", 5) == 0 || strncmp(bfr, "hi", 2) == 0) {
            strcpy(smsg, "Hello client!\n");
        } 
        else if (strncmp(bfr, "bye", 3) == 0) {
            strcpy(smsg, "Bye!\n");
            send(newsock, smsg, strlen(smsg), 0);
            break;  // Exit the loop if "bye" is received
        } 
        else if (strncmp(bfr, "how are you", 11) == 0) {
            strcpy(smsg, "I'm fine. How about you?\n");
        } 
        else {
            strcpy(smsg, "I'm just a simple bot. I don't know much!\n");
        }

        // Send response to client
        send(newsock, smsg, strlen(smsg), 0);
    }

    // Close connections
    close(newsock);
    close(serv);
    printf("Chatbot server closed.\n");
    return 0;
}
