//
// Created by re-get on 2020/12/26.
//
#include <iostream>
#include "SocketClient.h"

/**
 * @brief a small example of socket client, you can use
 * ssokit [http://product.rangaofei.cn/ssokit/pages/] to
 * test it
 */
int main() {
    // init Socket lib
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        errexit("WSAStartup failed\n");
        WSACleanup();
        return -1;
    }
    SocketClient client;
    char *msg = new char[65537 * 20];
    if (SocketClient::initClient(client, "127.0.0.1", 23414) == 0) {
        if (client.connect("127.0.0.1", 8080) == 0) {
            // send message
            client.send(msg, 65537 * 20);
            Sleep(300);
            // receive message
            char *rcvMsg = new char[100];
            client.recv(rcvMsg, 100);
            std::cout << rcvMsg << std::endl;
        }
    }
    // clear up Socket lib
    delete msg;
    WSACleanup();
    return 0;
}