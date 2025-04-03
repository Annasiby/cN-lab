#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char filename[BUFFER_SIZE], buffer[BUFFER_SIZE];
    int bytes_received;

    // Receive filename from the client
    bytes_received = recv(client_socket, filename, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving filename");
        close(client_socket);
        return;
    }
    filename[bytes_received] = '\0'; // Null terminate the received filename

    // Send server PID to the client
    snprintf(buffer, BUFFER_SIZE, "Server PID: %d\n", getpid());
    send(client_socket, buffer, strlen(buffer), 0);

    // Open the requested file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        char *error_message = "File not found";
        send(client_socket, error_message, strlen(error_message), 0);
        perror("File not found");
    } else {
        // Send file content to client
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            send(client_socket, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUFFER_SIZE);
        }
        printf("File sent to client\n");
        fclose(file);
    }

    // Close client connection
    close(client_socket);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pid_t pid;

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(server_sock);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("Error listening on socket");
        close(server_sock);
        exit(1);
    }

    printf("FTP server listening on port %d\n", PORT);

    // Main server loop
    while (1) {
        // Accept a client connection
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("Error accepting client connection");
            continue;
        }

        pid = fork(); // Create a child process for handling the client
        if (pid == 0) { // Child process
            close(server_sock); // Close server socket in child
            handle_client(client_sock); // Handle client
            exit(0); // Exit child process after handling client
        } else if (pid > 0) { // Parent process
            printf("Connected to client, PID: %d\n", pid);
            close(client_sock); // Close client socket in parent
        } else {
            perror("Fork failed");
        }
    }

    // Close server socket (this point is never reached in this example)
    close(server_sock);
    return 0;
}

