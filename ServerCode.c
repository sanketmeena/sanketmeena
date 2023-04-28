#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

int server_fd;
int new_socket;
struct sockaddr_in address;
int addrlen = sizeof(address);

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024] = {0};
    while (1) {
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0) {
            printf("Client disconnected\n");
            break;
        }
        printf("Client: %s", buffer);
        send(client_socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }
    close(client_socket);
    return NULL;
}

int main() {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started\n");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Client connected\n");
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, (void *)&new_socket);
    }

    close(server_fd);
    return 0;
}



/*gcc server.c -o server
./server
*/

//SERVER SIDE CODE TO START IT IN TERMINAL
//FOR MORE INFO USE CHATGPT
