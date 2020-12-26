//
// Created by re-get on 2020/12/26.
//

#ifndef SIMPLECLIENTSERVER_SOCKETCLIENT_H
#define SIMPLECLIENTSERVER_SOCKETCLIENT_H

#include "winsock2.h"
#include "SocketErr.h"

/**
 * @brief a simple server client
 */
class SocketClient {
public:
    // todo(add factory methods)
    SocketClient() {
        this->cSocket = INVALID_SOCKET;
        memset(&this->localSin, 0, sizeof(this->localSin));
        memset(&this->sSin, 0, sizeof(this->sSin));
    };

    ~SocketClient();

    /**
     * @brief init the tcp client by IP address and port
     * @param client the client to init
     * @param ipAddr the ip address to link
     * @param port the port to link
     * @return success: 0 | fail: -1
     */
    static int initClient(SocketClient &client, const char *ipAddr, unsigned short port);

    /**
     * @brief connect this client to server address
     * @return success: 0 | fail: -1
     */
    int connect();

    int connect(const char *ipAddr, unsigned short port);

    /**
     * @brief close the socket
     * @return
     */
    int close() const;

    /**
     * @brief send the message to server
     * @param buf: message address
     * @param len: message length
     * @return success: 0 | fail: -1
     */
    int send(char *buf, int len) const;

    /**
     * @brief recv the message from server
     * @param buf: message address
     * @param len: message length
     * @return success: 0 | fail: -1
     */
    int recv(char *buf, int len) const;

    /**
     * @brief covert a IP address and port to a new SOCKADDR_IN
     * @param ipAddr: the IP address to covert
     * @param port: the port to covert
     * @param sin: new SOCKADDR_IN
     * @return: a SOCKADDR_IN
     */
    static int ipAndPort2Sin(const char *ipAddr, unsigned short port, SOCKADDR_IN &sin);

private:
    SOCKET cSocket; // Client Socket
    SOCKADDR_IN localSin{}; // Client Addr
    SOCKADDR_IN sSin{}; // Server Addr
    CONST int BUFF_SIZE = 1024 * 64;

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


#endif //SIMPLECLIENTSERVER_SOCKETCLIENT_H
