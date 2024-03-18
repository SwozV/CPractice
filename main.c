#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUFFER_SIZE 1024

void analyzePacket(char* buffer, int size) {
    printf("Received packet: \n");
    for (int i = 0; i < size; i++) {
        printf("%02X ", (unsigned char)buffer[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    int addrLen = sizeof(struct sockaddr_in);
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // 创建套接字
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Failed to create socket.\n");
        return 1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8888);

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("Listening for incoming packets...\n");

    while (1) {
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &addrLen);
        if (bytesReceived == SOCKET_ERROR) {
            printf("recvfrom() failed.\n");
            break;
        }

        // 处理接收到的数据包
        analyzePacket(buffer, bytesReceived);
    }

    // 关闭套接字和Winsock
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
