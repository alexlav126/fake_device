#include "server.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include "logger.h"

#define SOCK_PATH "/tmp/sock_file"

Server::Server(proto::Proto::Callback callback)
{
    proto = new proto::Proto(callback);
    // if(proto == nullptr) throw ...
}

Server::~Server()
{
    Stop();
    delete proto;
}

bool Server::Start()
{
    struct sockaddr_un addr;
    int ret;

    if (thread_wait_connection_enable)
    {
        Stop();
    }

    Logger::Log(Logger::Level::INF, "Starting server...");
    serv_sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    Logger::Log(Logger::Level::DBG, "Server socket fd = %d", serv_sock_fd);
    if (serv_sock_fd == -1)
    {
        Logger::Log(Logger::Level::ERR, "Can't create socket");
        return false;
    }

    if (strlen(SOCK_PATH) > sizeof(addr.sun_path) - 1)
    {
        Logger::Log(Logger::Level::ERR,
                    "Server socket path too long: %s",
                    SOCK_PATH);
        return false;
    }

    // Delete any file that already exists at the address.
    // Make sure the deletion succeeds.
    // If the error is just that the file/directory doesn't exist, it's fine.
    if (remove(SOCK_PATH) == -1 && errno != ENOENT)
    {
        Logger::Log(Logger::Level::ERR,
                    "Can't remove %s",
                    SOCK_PATH);
        return false;
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    ret = bind(serv_sock_fd,
               (struct sockaddr *)&addr,
               sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        Logger::Log(Logger::Level::ERR, "Can't bind");
        return false;
    }

    const int BACKLOG = 5;
    if (listen(serv_sock_fd, BACKLOG) == -1)
    {
        Logger::Log(Logger::Level::ERR, "Can't listen");
        return false;
    }

    thread_wait_connection_enable = true;
    thread_wait_connection = new std::thread(&Server::WaitConnection,
                                             this);
    if (thread_wait_connection == nullptr)
    {
        Logger::Log(Logger::Level::ERR,
                    "Can't allocate thread_wait_connection");
        return false;
    }
    return true;
}

void Server::Stop()
{
    Logger::Log(Logger::Level::INF, "Stopping server...");
    if (thread_wait_connection_enable)
    {
        thread_wait_connection_enable = false;
        thread_wait_connection->join();
        delete thread_wait_connection;
    }

    // stop clients threads
    clients_lock.lock();
    for (auto it : clients)
    {
        it->thread_enable = false;
    }
    clients_lock.unlock();
    RemoveDisconnectedClients();
}

void Server::WaitConnection()
{
    struct pollfd fds;
    fds.fd = serv_sock_fd;
    fds.events = POLLIN;

    Logger::Log(Logger::Level::DBG, "Waiting to accept a connection...");

    while (thread_wait_connection_enable)
    {
        const int TIMEOUT_MS = 1000;
        int ret = poll(&fds, 1, TIMEOUT_MS);
        if (ret == -1)
        {
            Logger::Log(Logger::Level::ERR, "WaitConnection() poll error");
        }
        else if (ret == 0)
        {
            // timeout
        }
        else
        {
            if (fds.revents & POLLIN)
            {
                fds.revents = 0;
                int client_sock_fd = accept(serv_sock_fd, NULL, NULL);
                Logger::Log(Logger::Level::DBG, "Accepted socket fd = %d", client_sock_fd);
                AddClient(client_sock_fd);
            }
        }
        RemoveDisconnectedClients();
    }
}

void Server::ClientHandler(Server::Client *client)
{
    if (client == nullptr)
    {
        return;
    }

    struct pollfd fds;
    fds.fd = client->sock_fd;
    fds.events = POLLIN;
    ssize_t read_len;
    const int BUF_SIZE = 100;
    char buf[BUF_SIZE];

    Logger::Log(Logger::Level::INF,
                "Starting client %d handler",
                client->sock_fd);

    while (client->thread_enable)
    {
        const int TIMEOUT_MS = 1000;
        int ret = poll(&fds, 1, TIMEOUT_MS);
        if (ret == -1)
        {
            Logger::Log(Logger::Level::ERR, "ClientRead() poll error");
            break;
        }
        else if (ret == 0)
        {
            // timeout
        }
        else
        {
            if (fds.revents & POLLIN)
            {
                fds.revents = 0;
                read_len = read(client->sock_fd, buf, BUF_SIZE);
                if (read_len < 0)
                {
                    Logger::Log(Logger::Level::ERR,
                                "Client %d IO error",
                                client->sock_fd);
                    break;
                }
                if (read_len == 0)
                {
                    break;
                }

                // process data here
                buf[read_len] = 0;
                Logger::Log(Logger::Level::DBG,
                            "Client %d data: %s",
                            client->sock_fd,
                            buf);
                auto cmd = proto->CreateCommand(buf);
                if (cmd != nullptr)
                {
                    cmd->ProcessCallback();
                    auto data = cmd->GetResponse();
                    SendToClient(client->sock_fd, data);
                    delete cmd;
                }
            }
        }
    }

    Logger::Log(Logger::Level::INF,
                "Client %d close",
                client->sock_fd);

    if (close(client->sock_fd) == -1)
    {
        Logger::Log(Logger::Level::ERR,
                    "Client %d close error",
                    client->sock_fd);
    }

    client->thread_enable = false;
}

void Server::SendToClient(int client_sock_fd, const std::string &data)
{
    Logger::Log(Logger::Level::DBG,
                "Send to client %d data: %s",
                client_sock_fd,
                data.c_str());
    ssize_t len = data.size();
    if (write(client_sock_fd, data.c_str(), len) != len)
    {
        Logger::Log(Logger::Level::ERR,
                    "Can't send data to client %d",
                    client_sock_fd);
    }
}

void Server::AddClient(int client_sock_fd)
{
    Logger::Log(Logger::Level::INF,
                "Add client %d",
                client_sock_fd);

    Server::Client *client = new Server::Client();
    if (client == nullptr)
    {
        Logger::Log(Logger::Level::ERR,
                    "Client %d allocation error",
                    client_sock_fd);
        close(client_sock_fd);
        return;
    }

    client->sock_fd = client_sock_fd;
    client->thread_enable = true;
    client->thread_read = new std::thread(&Server::ClientHandler,
                                          this,
                                          client);
    if (client->thread_read == nullptr)
    {
        Logger::Log(Logger::Level::ERR,
                    "Client %d thread allocation error",
                    client_sock_fd);
        close(client_sock_fd);
        delete client;
        return;
    }
    clients_lock.lock();
    clients.push_back(client);
    clients_lock.unlock();
}

void Server::RemoveDisconnectedClients()
{
    // note: do not call it from ClientHandler thread
    clients_lock.lock();
    for (auto it = clients.begin(); it != clients.end();)
    {
        if ((*it)->thread_enable == false)
        {
            (*it)->thread_read->join();
            delete (*it)->thread_read;
            delete *it;
            it = clients.erase(it);
        }
        else
        {
            it = std::next(it);
        }
    }
    clients_lock.unlock();
}
