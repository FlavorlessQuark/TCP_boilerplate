#include "tcp.h"

int main() {
    int n;
    int sock;
    struct sockaddr_in  addr;
    WSADATA wsdata;
    char buffer[BUFF_SIZE];

    #ifdef _WIN32
        WSAStartup(MAKEWORD(2,2), &wsdata);
    #endif

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

    cleanup(sock);
    // #ifdef _WIN32
    //     closesocket(sock);
    //     WSACleanup();
    // #else
    //     close(sock);
    // #endif

    return 0;
}
