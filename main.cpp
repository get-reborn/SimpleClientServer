#include <iostream>
#include "SocketClient.h"

int main() {
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        errexit("WSAStartup failed\n");
        WSACleanup();
        return -1;
    }
    SocketClient client;
    if(SocketClient::initClient(client, "127.0.0.1", 23414) == 0) {
        if(client.connect("127.0.0.1", 8081) == 0) {
            //while(true) {
                char* msg = new char[65537 * 20];
                client.send(msg, 128020);
                Sleep(300);
            //}
        }
    }
    //client.connect("127.0.0.1", 8080);
    WSACleanup();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}