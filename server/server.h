#ifndef __SERVER_H_
#define __SERVER_H_

#include <thread>
#include <mutex>
#include <list>
#include <string>
#include "../proto/proto.h"

class Server
{
public:
    Server(proto::Proto::Callback callback);
    ~Server();

    bool Start();
    void Stop();

private:
    struct Client
    {
        int sock_fd;
        bool thread_enable;
        std::thread *thread_read;
    };

    void WaitConnection();
    void ClientHandler(Client *client);
    void SendToClient(int client_sock_fd, const std::string &data);
    void AddClient(int client_sock_fd);
    void RemoveDisconnectedClients();
    int serv_sock_fd;

    std::thread *thread_wait_connection = nullptr;
    bool thread_wait_connection_enable = false;

    std::mutex clients_lock;
    std::list<Client *> clients;

    proto::Proto *proto;
};

#endif // __SERVER_H_