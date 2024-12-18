#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <winsock2.h>
#include <time.h>

#define MAX_CLIENTS 5
#define NUM_QUESTIONS 5
#define PORT 12346

struct Client {
    int socket;
    char name[256];
    int score;
};

struct Question {
    char question[512];
    char options[4][128];
    char correct_option; // 'a', 'b', 'c', or 'd'
};

struct Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t lock;

struct Question questions[NUM_QUESTIONS] = {
    {"What is the capital of France?", {"a) Berlin", "b) Madrid", "c) Paris", "d) Rome"}, 'c'},
    {"Which planet is known as the Red Planet?", {"a) Earth", "b) Mars", "c) Jupiter", "d) Venus"}, 'b'},
    {"Who wrote 'Romeo and Juliet'?", {"a) Charles Dickens", "b) William Shakespeare", "c) Mark Twain", "d) Leo Tolstoy"}, 'b'},
    {"What is the largest ocean on Earth?", {"a) Atlantic", "b) Indian", "c) Arctic", "d) Pacific"}, 'd'},
    {"How many continents are there?", {"a) 5", "b) 6", "c) 7", "d) 8"}, 'c'}
};

void *handle_client(void *arg);
void broadcast_scores();
void safe_send(int socket, const char *message);

int main() {
    WSADATA wsaData;
    struct sockaddr_in server_addr, client_addr;
    int server_socket, client_socket;
    int client_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Quiz server started on port %d\n", PORT);

    pthread_mutex_init(&lock, NULL);

    while (client_count < MAX_CLIENTS) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            printf("Accept failed\n");
            continue;
        }

        pthread_mutex_lock(&lock);
        clients[client_count].socket = client_socket;
        clients[client_count].score = 0;
        client_count++;
        pthread_mutex_unlock(&lock);

        pthread_create(&threads[client_count - 1], NULL, handle_client, &clients[client_count - 1]);
        printf("Client connected. Total clients: %d\n", client_count);
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

void *handle_client(void *arg) {
    struct Client *client = (struct Client *)arg;
    char buffer[512];
    int bytes_received;

    // Receive client name
    bytes_received = recv(client->socket, client->name, sizeof(client->name), 0);
    if (bytes_received <= 0) {
        printf("Failed to receive client name. Disconnecting client.\n");
        closesocket(client->socket);
        return NULL;
    }
    client->name[bytes_received] = '\0'; // Null-terminate the name
    printf("Client %s joined the quiz\n", client->name);

    // Send questions
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        sprintf(buffer, "Question %d: %s\n%s\n%s\n%s\n%s\nYour answer (a/b/c/d): ",
                i + 1, questions[i].question,
                questions[i].options[0], questions[i].options[1],
                questions[i].options[2], questions[i].options[3]);
        safe_send(client->socket, buffer);

        // Receive answer
        bytes_received = recv(client->socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            printf("Client %s disconnected unexpectedly.\n", client->name);
            closesocket(client->socket);
            return NULL;
        }
        buffer[bytes_received] = '\0'; // Null-terminate input

        // Validate and process answer
        char user_answer = buffer[0];
        if (user_answer == questions[i].correct_option) {
            client->score++;
            safe_send(client->socket, "Correct!\n");
        } else {
            sprintf(buffer, "Wrong! The correct answer was '%c'\n", questions[i].correct_option);
            safe_send(client->socket, buffer);
        }
    }

    // Broadcast scores after quiz
    pthread_mutex_lock(&lock);
    broadcast_scores();
    pthread_mutex_unlock(&lock);

    closesocket(client->socket);
    return NULL;
}

void broadcast_scores() {
    char buffer[512];
    sprintf(buffer, "\n--- Final Scores ---\n");
    for (int i = 0; i < client_count; i++) {
        sprintf(buffer + strlen(buffer), "%s: %d points\n", clients[i].name, clients[i].score);
    }

    for (int i = 0; i < client_count; i++) {
        safe_send(clients[i].socket, buffer);
    }

    printf("%s", buffer);
}

void safe_send(int socket, const char *message) {
    if (send(socket, message, strlen(message), 0) == -1) {
        printf("Failed to send message to socket %d\n", socket);
    }
}
