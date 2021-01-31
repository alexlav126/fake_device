#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>
#include <string>

#define SOCK_PATH "/tmp/sock_file"

static void HandlerLoop(int sfd)
{
    struct pollfd fds;
    fds.fd = sfd;
    fds.events = POLLIN;
    ssize_t read_len;
    const int BUF_SIZE = 100;
    char buf[BUF_SIZE];

    const int NUM_FAST_CMD = 5;
    const char *FAST_CMD[NUM_FAST_CMD] = {
        "start_measure channel0\n",
        "stop_measure channel0\n",
        "set_range channel0, range2\n",
        "get_status channel0\n",
        "get_result channel0\n",
    };

    bool loop_enable = true;

    while (loop_enable)
    {
        printf("\n\n-------------\n");
        printf("Fast commands:\n");
        for (int i = 0; i < NUM_FAST_CMD; i++)
        {
            printf("%d - %s", i, FAST_CMD[i]);
        }
        printf("Enter command:\n");

        // read from STDIN
        read_len = read(STDIN_FILENO, buf, BUF_SIZE);
        if (read_len > 0)
        {
            if (read_len == 2)
            {
                buf[read_len] = 0;
                int i = std::stoi(buf);
                if (i >= 0 && i < NUM_FAST_CMD)
                {
                    strcpy(buf, FAST_CMD[i]);
                    read_len = strlen(FAST_CMD[i]);
                    buf[read_len] = 0;
                    printf("fast command is:%s", buf);
                }
            }
            printf("write to server\n");
            if (write(sfd, buf, read_len) != read_len)
            {
                printf("write failed\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            printf("read STDIN failed\n");
            exit(EXIT_FAILURE);
        }

        printf("read from server\n");
        const int TIMEOUT_MS = 1000;
        int ret = poll(&fds, 1, TIMEOUT_MS);
        if (ret == -1)
        {
            printf("poll error\n");
            exit(EXIT_FAILURE);
        }
        else if (ret == 0)
        {
            printf("timeout\n");
        }
        else
        {
            if (fds.revents & POLLIN)
            {
                fds.revents = 0;
                read_len = read(sfd, buf, BUF_SIZE);
                if (read_len < 0)
                {
                    printf("IO error\n");
                    break;
                }
                if (read_len == 0)
                {
                    break;
                }

                // process data here
                buf[read_len] = 0;
                printf("data from server:%s", buf);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct sockaddr_un addr;

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Client socket fd = %d\n", sfd);

    if (sfd == -1)
    {
        printf("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sfd,
                (struct sockaddr *)&addr,
                sizeof(struct sockaddr_un)) == -1)
    {
        printf("connect error");
        exit(EXIT_FAILURE);
    }

    HandlerLoop(sfd);

    exit(EXIT_SUCCESS);
}