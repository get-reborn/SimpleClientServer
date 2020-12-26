//
// Created by re-get on 2020/12/26.
//

#ifndef SIMPLECLIENTSERVER_SOCKETCLIENT_H
#define SIMPLECLIENTSERVER_SOCKETCLIENT_H

#include "winsock2.h"
#include "SocketErr.h"

/**
 * @brief a simple tcp client
 */
class SocketClient {
public:
    SocketClient() {
        this->cSocket = INVALID_SOCKET;
        memset(&this->localSin, 0, sizeof(this->localSin));
        memset(&this->sSin, 0, sizeof(this->sSin));
    };

    ~SocketClient() {};


    static int initClient(SocketClient &client, const char *ipAddr, unsigned short port);

    /**
     * @brief connect this client to server address
     * @param sSin: the address about server
     * @return success: 0 | fail: -1
     */
    int connect();

    int connect(const char *ipAddr, unsigned short port);

    /**
     * @brief send the message to server
     * @param buf: message address
     * @param len: message length
     * @return success: 0 | fail: -1
     */
    int send(char *buf, int len);

    /**
     * @brief recv the message from server
     * @param buf: message address
     * @param len: message length
     * @return success: 0 | fail: -1
     */
    int recv(char *buf, int len);

private:
    SOCKET cSocket; // Client Socket
    SOCKADDR_IN localSin; // Client Addr
    SOCKADDR_IN sSin; // Server Addr
    CONST int BUFF_SIZE = 1024 * 64;

    /**
     * @brief create a TCP SOCKET
     * @param socket: a new TCP SOCKET
     * @return: success: 0 | fail: -1
     */
    int createTcpSocket(SOCKET &cSocket);

    /**
     * @brief bind a IP address and port to a SOCKET
     * @param socket: the socket to bind
     * @param sin: the IP address and port
     * @return: success: 0 | fail: -1
     */
    int bind(SOCKET &socket, SOCKADDR_IN &sin);

    /**
     * @brief covert a IP address and port to a new SOCKADDR_IN
     * @param ipAddr: the IP address to covert
     * @param port: the port to covert
     * @param sin: new SOCKADDR_IN
     * @return: a SOCKADDR_IN
     */
    int ipAndPort2Sin(const char *ipAddr, unsigned short port, SOCKADDR_IN &sin);

};


#endif //SIMPLECLIENTSERVER_SOCKETCLIENT_H
