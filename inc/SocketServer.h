//
// Created by re-get on 2020/12/26.
//

#ifndef SIMPLECLIENTSERVER_SOCKETSERVER_H
#define SIMPLECLIENTSERVER_SOCKETSERVER_H


#include "winsock2.h"
#include "SocketErr.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <thread>
#include <algorithm>
#include <cctype>
#include <mutex>

using namespace std;

struct accepted_client {
    SOCKET aSocket;
    SOCKADDR_IN sSin{};

    accepted_client() {
        aSocket = INVALID_SOCKET;
        memset(&sSin, 0, sizeof(sSin));
    }

};

class SocketServer {
public:
    SocketServer();

    ~SocketServer();

    /**
     * @brief init the tcp server by IP address and port
     * @param client the server to init
     * @param ipAddr the ip address to link
     * @param port the port to link
     * @return success: 0 | fail: -1
     */
    int initServer(SocketServer &server, const char *ipAddr, unsigned short port);

    /**
     * @brief send the msg to all connect client
     * @param buf: the message to send
     * @param len: the length to send
     * @return success: 0 | fail: -1
     */
    int sendToAll(char *buf, int len);

    /**
     * @brief send the msg to one client by id
     * @param buf: the message to send
     * @param len: the length to send
     * @return success: 0 | fail: -1
     */
    int sendToOne(int id, char *buf, int len);

    /**
     * @brief receive the msg to one client by id
     * @param buf: the message to send
     * @param len: the length to send
     * @return success: 0 | fail: -1
     */
    int recvFromOne(int id, char *buf, int len);

    /**
     * @brief accept the connect
     * @return success: 0 | fail: -1
     */
    int accept();

    /**
     * @brief remove a connected client by id
     * @return success: 0 | fail: -1
     */
    int detach(int id);

    /**
     * @brief remove all client
     * @return success: 0 | fail: -1
     */
    int clean();

    /**
     * @brief close the server and clean up all the connect
     * @return success 0 | fail -1
     */
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
    CONST static int MIN_VECTOR_SIZE = 16;
    SOCKET sSocket; // Server Socket
    SOCKADDR_IN localSin; // Client Addr
    vector<accepted_client> acceptedClients;
    // pthread_t serverThread[MAX_CLIENT];

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

    /**
     * @brief shift the state to listen
     * @return: success: 0 | fail: -1
     */
    int listen();
};

#endif //SIMPLECLIENTSERVER_SOCKETSERVER_H
