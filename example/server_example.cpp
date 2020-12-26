//
// Created by re-get on 2020/12/26.
//

#include <iostream>
#include <csignal>
#include <ctime>
#include "SocketServer.h"

int main(int argc, char **argv)
{
    // init Socket lib
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        errexit("WSAStartup failed\n");
        WSACleanup();
        return -1;
    }
    SocketServer server;
    char *msg = new char[65537 * 20];
    if (SocketServer::initServer(server, "127.0.0.1", 8080) == 0) {
        while(true) {
            if (server.accept() == -1) {
                break;
            }
            // send msg
            if (server.sendToAll(msg, 65537 * 20) == -1) {
                break;
            }
            Sleep(1000);
        }
    }
    // clear up Socket lib
    WSACleanup();
    return 0;
}