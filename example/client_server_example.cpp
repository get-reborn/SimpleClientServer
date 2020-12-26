//
// Created by re-get on 2020/12/26.
//
#include <iostream>
#include "SocketClient.h"
#include "SocketServer.h"

/**
 * @brief a small example of socket client and server
 */
string getDateStr() {
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    int hour = now->tm_hour;
    int min = now->tm_min;
    int sec = now->tm_sec;
    std::string date =
            to_string(now->tm_year + 1900) + "-" +
            to_string(now->tm_mon + 1) + "-" +
            to_string(now->tm_mday) + " " +
            to_string(hour) + ":" +
            to_string(min) + ":" +
            to_string(sec) + "\r\n";
    return date;
}

void *server_thread(void *m) {
    SocketServer server;
    if (SocketServer::initServer(server, "127.0.0.1", 8080) == 0) {
        while (true) {
            if (server.accept() == -1) {
                break;
            }
            // receive msg
            while (true) {
                char *rcvMsg = new char[200];
                if (server.recvFromOne(0, rcvMsg, 200) == -1) {
                    break;
                }
                std::cout << rcvMsg << std::endl;
            }
            // send msg
//            std::string date = getDateStr();
//            int len = date.length() + 1;
//            char *msg = new char[len];
//            strcpy_s(msg, len, date.c_str());
//            while (true) {
//                if (server.sendToAll(msg, len) == -1) {
//                    break;
//                }
//                Sleep(5000);
//            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    // init Socket lib
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        errexit("WSAStartup failed\n");
        WSACleanup();
        return -1;
    }

    pthread_t serverThread;
    if (pthread_create(&serverThread, NULL, server_thread, (void *) 0) == 0) {
        SocketClient client;
        if (SocketClient::initClient(client, "127.0.0.1", 23414) == 0) {
            if (client.connect("127.0.0.1", 8080) == 0) {
                while (true) {
                    // send message
                    std::string date = getDateStr();
                    int len = date.length() + 1;
                    char *msg = new char[len];
                    strcpy_s(msg, len, date.c_str());
                    if (client.send(msg, len) == -1) {
                        break;
                    }
                    Sleep(5000);
                    // receive message
//                    char *rcvMsg = new char[200];
//                    if (client.recv(rcvMsg, 200) == -1) {
//                        break;
//                    }
//                    std::cout << rcvMsg << std::endl;
                }
            }
        }
    }
    WSACleanup();
    return 0;
}