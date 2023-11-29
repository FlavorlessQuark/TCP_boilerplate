#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "debug.h"

#define VALIDATE(_return, success, err) { \
    if (_return < 0)    \
        DEBUG(err); \
    else                \
        DEBUG(success);      \
        }

# define IP "127.0.0.1"
# define PORT 8080
# define BUFF_SIZE 1024

int main(){

    int serv_sock, cli_sock;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_size;
    char buffer[BUFF_SIZE];
    int n;

    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = PORT;
    serv_addr.sin_addr.s_addr = inet_addr(IP);

    VALIDATE((serv_sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket")
    // n = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    VALIDATE((n = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))), "Bind success", "Bind fail");
    listen(serv_sock, 5);
    printf("Listening...\n");

    while(1){
        addr_size = sizeof(cli_addr);
        cli_sock = accept(serv_sock, (struct sockaddr*)&cli_addr, &addr_size);
        printf("Client connected.\n");

        memset(buffer, '\0', sizeof(buffer));
        recv(cli_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        memset(buffer, '\0', sizeof(buffer));
        strcpy(buffer, "HI from server :0");
        send(cli_sock, buffer, strlen(buffer), 0);

        // close(client_sock);
        // printf("[+]Client disconnected.\n\n");

    }
    return 0;
}
