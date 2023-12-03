#include "tcp.h"

int main(){

    int sock, cli_sock;
    struct sockaddr_in serv_addr, cli_addr;
    int addr_size;

    char buffer[BUFF_SIZE];
    int n;

    #ifdef _WIN32
    	WSDATA wsdata;
        WSAStartup(MAKEWORD(2,2), &wsdata);
    #endif

    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = PORT;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    VALIDATE((sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket")
    // n = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    VALIDATE((n = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))), "Bind success", "Bind fail");
    listen(sock, 5);
    printf("Listening...\n");

    while(1){
        addr_size = sizeof(cli_addr);
        cli_sock = accept(sock, (struct sockaddr*)&cli_addr, &addr_size);
        printf("Client connected.\n");

        memset(buffer, '\0', sizeof(buffer));
        recv(cli_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        memset(buffer, '\0', sizeof(buffer));
        strcpy(buffer, "HI from server :0");
        send(cli_sock, buffer, strlen(buffer), 0);

        //cleanup(cli_sock);

    }
    return 0;
}
