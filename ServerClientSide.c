#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int client_fd;
struct sockaddr_in serv_addr;

int main() {
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    char buffer[1024] = {0};
    while (1) {
        printf("Enter message: ");
        fgets(buffer, 1024, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
        int valread = read(client_fd, buffer, 1024);
        printf("Server: %s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    close(client_fd);
    return 0;
}


/*gcc client.c -o client
./client
*/

//TO STATRT THE CODE ON THE TERMINAL
