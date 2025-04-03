#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char filename[BUFFER_SIZE], buffer[BUFFER_SIZE];
    ssize_t bytes_sent, bytes_received;

    // Create client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        close(sock);
        exit(1);
    }

    // Get filename from user
    printf("Enter the filename to request: ");
    fgets(filename, BUFFER_SIZE, stdin);
    filename[strcspn(filename, "\n")] = '\0';  // Remove newline character

    // Send filename to server
    bytes_sent = send(sock, filename, strlen(filename), 0);
    if (bytes_sent < 0) {
        perror("Error sending filename to server");
        close(sock);
        exit(1);
    }

    // Receive server PID
    bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving server PID");
        close(sock);
        exit(1);
    }
    buffer[bytes_received] = '\0';
    printf("Server Response: %s\n", buffer);

    // Receive the file content or error message
    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), bytes_received, stdout);
    }

    if (bytes_received < 0) {
        perror("Error receiving file content");
    }

    // Close socket after receiving the file
    close(sock);
    return 0;
}

