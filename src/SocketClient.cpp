//
// Created by re-get on 2020/12/26.
//

#include <io.h>
#include "SocketClient.h"

SocketClient::~SocketClient() {
    close();
}

int SocketClient::initClient(SocketClient &client, const char *ipAddr, unsigned short port) {
    // create socket if socket invalid
    if (client.cSocket == INVALID_SOCKET) {
        if (client.createTcpSocket() == -1) {
            return -1;
        }
    }
    // covert ip and port to SOCKADDR_IN
    if (client.ipAndPort2Sin(ipAddr, port, client.localSin) == -1) {
        return -1;
    }
    // bind the address and socket
    client.bind();
    return 0;
}

int SocketClient::connect() {
    if (::connect(this->cSocket, (struct sockaddr *) &sSin, sizeof(sSin)) == SOCKET_ERROR) {
        errexit("can't connect to server: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketClient::connect(const char *ipAddr, unsigned short port) {
    if (ipAndPort2Sin(ipAddr, port, this->sSin) == -1) {
        return -1;
    }
    if (connect() == -1) {
        return -1;
    }
    return 0;
}

int SocketClient::close() const {
    if(::close(this->cSocket) == -1){
        errexit("can't close socket: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketClient::send(char *buf, int len) const {
    if (this->cSocket == INVALID_SOCKET) {
        errexit("can't send entry: %d\n", GetLastError());
        return -1;
    }
    if (len <= BUFF_SIZE) {
        if (::send(this->cSocket, buf, len, 0) == SOCKET_ERROR) {
            errexit("can't send entry: %d\n", GetLastError());
            return -1;
        }
    } else {
        int idx = 0;
        for (; idx < len - BUFF_SIZE; idx += BUFF_SIZE) {
            if (::send(this->cSocket, buf + idx, BUFF_SIZE, 0) == SOCKET_ERROR) {
                errexit("can't send entry: %d\n", GetLastError());
                return -1;
            }
        }
        if (::send(this->cSocket, buf + idx, len - idx, 0) == SOCKET_ERROR) {
            errexit("can't send entry: %d\n", GetLastError());
            return -1;
        }
    }
    return 0;
}

int SocketClient::recv(char *buf, int len) const {
    if (cSocket == INVALID_SOCKET) {
        errexit("can't recv entry: %d\n", GetLastError());
        return -1;
    }
    memset(buf, 0, len);
    if (::recv(cSocket, buf, len, 0) == SOCKET_ERROR) {
        errexit("can't recv entry: %d\n", GetLastError());
        return -1;
    } else {
        return 0;
    }
}

int SocketClient::createTcpSocket() {
    if (this->cSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP) == INVALID_SOCKET) {
        errexit("can't create socket: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketClient::bind() {
    if (::bind(this->cSocket, (struct sockaddr *) &this->localSin, sizeof(this->localSin)) == SOCKET_ERROR) {
        errexit("can't bind address: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketClient::ipAndPort2Sin(const char *ipAddr, unsigned short port, SOCKADDR_IN &sin) {
    sin.sin_family = AF_INET;
    // Map ipAddr to IP address
    if ((sin.sin_addr.S_un.S_addr = inet_addr(ipAddr)) == INADDR_NONE) {
        errexit("can't get \"%s\" ipAddr entry: %d\n", ipAddr, GetLastError());
        return -1;
    }
    if ((sin.sin_port = htons((u_short) port)) == 0) {
        errexit("can't get \"%s\" port entry: %d\n", port, GetLastError());
        return -1;
    }
    return 0;
}