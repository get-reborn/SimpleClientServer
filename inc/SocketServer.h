//
// Created by re-get on 2020/12/26.
//

#ifndef SIMPLECLIENTSERVER_SOCKETSERVER_H
#define SIMPLECLIENTSERVER_SOCKETSERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "winsock2.h"
#include <string.h>
#include <pthread.h>
#include <thread>
#include <algorithm>
#include <cctype>
#include <mutex>

using namespace std;

struct accepted_client {
    int id;
    SOCKET aSocket;
    SOCKADDR_IN sSin;

    accepted_client() {
        id = -1;
        aSocket = INVALID_SOCKET;
        memset(&sSin, 0, sizeof(sSin));
    }

};

class SocketServer {
public:
    SocketServer() {}

    ~SocketServer() {
        close();
    }

    int initServer(SocketServer &server, const char *ipAddr, unsigned short port);

    int sendToAll(const char *msg, int len);

    int sendToOne(accepted_client &client, const char *msg, int len);

    int recvFromOne(accepted_client &client, const char *msg, int len);

    int accepted();

    int detach(int id);

    int clean(int id);

    int close();
    /**
     * @brief covert a IP address and port to a new SOCKADDR_IN
     * @param ipAddr: the IP address to covert
     * @param port: the port to covert
     * @param sin: new SOCKADDR_IN
     * @return: a SOCKADDR_IN
     */
    static int ipAndPort2Sin(const char *ipAddr, unsigned short port, SOCKADDR_IN &sin);

private:
    CONST static int BUFF_SIZE = 1024 * 64;
    CONST static int MAX_CLIENT = 1000;
    SOCKET sSocket; // Server Socket
    SOCKADDR_IN localSin; // Client Addr
    vector<accepted_client> acceptedClients;
    pthread_t  serverThread[ MAX_CLIENT ];

    /**
    * @brief create a TCP SOCKET
    * @return: success: 0 | fail: -1
    */
    int createTcpSocket();

    /**
     * @brief bind a IP address and port to a SOCKET
     * @return: success: 0 | fail: -1
     */
    int bind();
};

#endif //SIMPLECLIENTSERVER_SOCKETSERVER_H
