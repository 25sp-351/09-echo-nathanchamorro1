#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RECVBUFSIZE 1024

// Prints error message and exits the program
void error(const char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Checks for valid number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in server_addr;
    char buffer[RECVBUFSIZE];
    int port = atoi(argv[2]);

    // Creates a TCP socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error("Socket");
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address string to binary
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        error("IP address conversion");
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        error("Connect");
    }

    printf("Connected to server %s on port %d\n", argv[1], port);

    // Communication loop
    while (1)
    {
        printf("Enter message: ");
        
        // Read input
        if (fgets(buffer, RECVBUFSIZE, stdin) == NULL)
        {
            break;
        }

        // Send message to server
        if (send(sock, buffer, strlen(buffer), 0) < 0)
        {
            error("Send");
        }

        // Receive and print response from server
        int bytes_received = recv(sock, buffer, RECVBUFSIZE - 1, 0);
        if (bytes_received <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server: %s", buffer);
    }

    // Close the socket
    close(sock);
    return 0;
}
