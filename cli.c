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

int main() {
    int n;
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[BUFF_SIZE];




    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr(IP);

    VALIDATE((sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket")
    VALIDATE(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "Connected", "Failed to connect")

    memset(buffer, '\0', sizeof(buffer));
    strcpy(buffer, "Hello from client :)");
    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, '\0', sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    close(sock);

    return 0;
}
