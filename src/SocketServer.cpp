#include "SocketServer.h"

SocketServer::SocketServer() {
    sSocket = INVALID_SOCKET;
    memset(&this->localSin, 0, sizeof(this->localSin));
    acceptedClients.reserve(MIN_VECTOR_SIZE);
}

SocketServer::~SocketServer() {
    close();
}


int SocketServer::initServer(SocketServer &server, const char *ipAddr, unsigned short port) {
    // create socket if socket invalid
    if (server.sSocket == INVALID_SOCKET) {
        if (server.createTcpSocket() == -1) {
            return -1;
        }
    }
    // covert ip and port to SOCKADDR_IN
    if (server.ipAndPort2Sin(ipAddr, port, server.localSin) == -1) {
        return -1;
    }
    // bind the address and socket
    if (server.bind() == -1) {
        return -1;
    }
    // shift to listen
    if (server.listen() == -1) {
        return -1;
    }
    return 0;
}

int SocketServer::sendToAll(char *buf, int len) {
    for (int i = 0; i < acceptedClients.size(); i++) {
        if (sendToOne(i, buf, len) == -1) {
            errexit("can't send to one\n");
            return -1;
        }
    }
}

int SocketServer::sendToOne(int id, char *buf, int len) {
    if (id > acceptedClients.size()) {
        errexit("size out of vector \n");
        return -1;
    }
    accepted_client acc_client = acceptedClients[id];
    if (acc_client.aSocket == INVALID_SOCKET) {
        errexit("can't send entry: %d\n", GetLastError());
        return -1;
    }
    if (len <= BUFF_SIZE) {
        if (::send(acc_client.aSocket, buf, len, 0) == SOCKET_ERROR) {
            errexit("can't send entry: %d\n", GetLastError());
            return -1;
        }
    } else {
        int idx = 0;
        for (; idx < len - BUFF_SIZE; idx += BUFF_SIZE) {
            if (::send(acc_client.aSocket, buf + idx, BUFF_SIZE, 0) == SOCKET_ERROR) {
                errexit("can't send entry: %d\n", GetLastError());
                return -1;
            }
        }
        if (::send(acc_client.aSocket, buf + idx, len - idx, 0) == SOCKET_ERROR) {
            errexit("can't send entry: %d\n", GetLastError());
            return -1;
        }
    }
    return 0;
}

int SocketServer::recvFromOne(int id, char *buf, int len) {
    if (id > acceptedClients.size()) {
        errexit("size out of vector \n");
        return -1;
    }
    accepted_client acc_client = acceptedClients[id];
    if (acc_client.aSocket == INVALID_SOCKET) {
        errexit("can't recv entry: %d\n", GetLastError());
        return -1;
    }
    memset(buf, 0, len);
    if (::recv(acc_client.aSocket, buf, len, 0) == SOCKET_ERROR) {
        errexit("can't recv entry: %d\n", GetLastError());
        return -1;
    } else {
        return 0;
    }
    return 0;
}

int SocketServer::accept() {
    accepted_client accClient;
    int len = sizeof(accClient);
    accClient.aSocket = ::accept(this->sSocket, (struct sockaddr *) &accClient.sSin, &len);
    if (accClient.aSocket == INVALID_SOCKET) {
        errexit("accept failed: %d\n", GetLastError());
        return -1;
    }
    this->acceptedClients.push_back(accClient);
    return 0;
}

int SocketServer::detach(int id) {
    if (id > acceptedClients.size()) {
        errexit("size out of vector \n");
        return -1;
    }
    accepted_client accClient = acceptedClients[id];
    if (::close(accClient.aSocket) == -1) {
        errexit("can't close socket: %d\n", GetLastError());
        return -1;
    }
    // remove this client
    vector<accepted_client>::iterator it = acceptedClients.begin();
    for (; it != it + id; it++);
    acceptedClients.erase(it);
    return 0;
}

int SocketServer::clean() {
    for (accepted_client accClient : acceptedClients) {
        if (::close(accClient.aSocket) == -1) {
            errexit("can't close socket: %d\n", GetLastError());
            return -1;
        }
    }
    acceptedClients.empty();
    return 0;
}

int SocketServer::close() {
    if (clean() == -1) {
        return -1;
    }
    if (::close(this->sSocket) == -1) {
        errexit("can't close socket: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketServer::ipAndPort2Sin(const char *ipAddr, unsigned short port, SOCKADDR_IN &sin) {
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

int SocketServer::createTcpSocket() {
    if ((this->sSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        errexit("can't create socket: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketServer::bind() {
    if (::bind(this->sSocket, (struct sockaddr *) &this->localSin, sizeof(this->localSin)) == SOCKET_ERROR) {
        errexit("can't bind address: %d\n", GetLastError());
        return -1;
    }
    return 0;
}

int SocketServer::listen() {
    if (::listen(this->sSocket, 5) == SOCKET_ERROR) {
        errexit("can't shift the state to listen : %d\n", GetLastError());
        return -1;
    }
    return 0;
}
