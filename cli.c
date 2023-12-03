#include "tcp.h"

int main() {
    struct sockaddr_in  addr;
    char buffer[BUFF_SIZE];

    #ifdef _WIN32
        WSADATA wsdata;
        SOCKET sock;
        if ( WSAStartup(MAKEWORD(2,2), &wsdata)== SOCKET_ERROR){
            printf ("Error initialising WSA.\n");
            return -1;
        }
    #else
        int sock;
    #endif

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr("10.0.0.137");

    VALIDATE((sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket %d")
    // VALIDATE(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "Connected", "Failed to connect %d")
    int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        printf("Couldnt connect ret %d errno %d\n", ret, err);
        cleanup(sock);
    }
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
