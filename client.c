#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 12346

int main() {
    WSADATA wsaData;
    struct sockaddr_in server_addr;
    int client_socket;
    char buffer[512];
    char name[256];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Connection to server failed\n");
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Send player name
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strlen(name) - 1] = '\0'; // Remove newline
    send(client_socket, name, strlen(name), 0);

    // Receive questions and send answers
    for (int i = 0; i < 5; i++) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer before receiving each question
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("\n%s", buffer);

        printf("Your choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove newline from input
        send(client_socket, buffer, strlen(buffer), 0); // Send answer without newline

        memset(buffer, 0, sizeof(buffer)); // Clear buffer again to receive the result
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("%s\n", buffer);
    }

    // Receive final scores
    memset(buffer, 0, sizeof(buffer)); // Clear buffer before receiving final scores
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("%s", buffer);

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
