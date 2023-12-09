#include "tcp.h"

int main() {
    int ret;
    struct sockaddr_in  addr;
    char buffer[30];

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

    #ifdef _WIN32
        u_long mode = 1;

        VALIDATE((sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket %d")
        ret = ioctlsocket(sock, FIONBIO, &mode);
        printf("nonblock returend %d\n", ret);
    #else
        VALIDATE((sock = socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK)), "Socket created", "Failed to create socket %d")
    // VALIDATE(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "Connected", "Failed to connect %d")
    #endif

    int retry = 0;
    int err;
    do {
        ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        if (ret == SOCKET_ERROR && (err = WSAGetLastError()) != WSAEWOULDBLOCK)
        {            
            printf("Couldnt connect ret %d errno %d\n", ret, err);
            cleanup(sock);
        }
        else {
            printf("COnnected");
            break ;
        }
        Sleep(1000);
        retry++;
    } while (retry < 10 && ret == SOCKET_ERROR);

    // if (ret == SOCKET_ERROR)
    // {
    //     int err = WSAGetLastError();
    //     printf("Couldnt connect ret %d errno %d\n", ret, err);
    //     cleanup(sock);
    // }
    int b;
    while (1) {
        memset(buffer, '\0', sizeof(buffer));
        while (fgets(buffer, BUFF_SIZE - 1, stdin) != NULL)
        {
            ret = send(sock, buffer, strlen(buffer), 0);
            DEBUG("Send returned %d\n", ret);
            break ;
        }
        printf("Memset\n");
        memset(buffer, '\0', sizeof(buffer));
        ret = recv(sock, buffer, sizeof(buffer), 0);
        DEBUG("Recv returned %d\n", ret);
        printf("Server: %s\n", buffer);
    }

    cleanup(sock);
    // #ifdef _WIN32
    //     closesocket(sock);
    //     WSACleanup();
    // #else
    //     close(sock);
    // #endif

    return 0;
}
