#ifndef CAN_CONTROL_H
#define CAN_CONTROL_H
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <QDebug>
typedef __u8    byte;
static int send_socket_fd = -1;
static int recv_socket_fd = -1;

class Can_Control
{
public:
    Can_Control();
    int can_send();
    int can_recv();
    int socket_connect(const char *m_port);
    void socket_close(const int sockfd);
    void set_can_filter(const int sockfd);
    void set_can_loopback(const int sockfd, bool sta);
    int send_frame(int sockfd, const void* data, const int count);
    int recv_frame(int sockfd, struct can_frame* buf, const int count, const int timeout_ms);

    int stSocket;
};

#endif // CA_CONTROL_H
