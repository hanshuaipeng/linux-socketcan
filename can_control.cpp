#include "can_control.h"
#include <QMessageBox>

Can_Control::Can_Control()
{

}

int Can_Control::can_send()
{
    int stSocket_LO, stSend_LO;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame[2] = {{0}};

    stSocket_LO = socket(PF_CAN,SOCK_RAW,CAN_RAW);
    strcpy(ifr.ifr_name,"can1");
    ioctl(stSocket_LO,SIOCGIFINDEX,&ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(stSocket_LO,(struct sockaddr*)&addr, sizeof(addr));
    setsockopt(stSocket_LO, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    frame[0].can_id = 0x11;
    frame[0].can_dlc = 1;
    frame[0].data[0] = '1';
    frame[1].can_id = 0x22;
    frame[1].can_dlc = 1;
    frame[1].data[0] = '2';

    while (1)
    {
        stSend_LO = write(stSocket_LO,&frame[0],sizeof(frame[0]));
        if(stSend_LO != sizeof(frame[0]))
        {
            qDebug()<<"send error frame[0]";
            break;
        }
        sleep(1);
        stSend_LO = write(stSocket_LO,&frame[1],sizeof(frame[1]));
        if(stSend_LO != sizeof(frame[1]))
        {
            qDebug()<<"send error frame[1]";
            break;
        }
        sleep(1);
    }
    close(stSocket_LO);
    return 0;
}

int Can_Control::can_recv()
{
    int stSocket_LO, stRecv_LO;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[1];
    stSocket_LO = socket(PF_CAN,SOCK_RAW,CAN_RAW);
    strcpy(ifr.ifr_name,"can1");
    ioctl(stSocket_LO,SIOCGIFINDEX,&ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(stSocket_LO,(struct sockaddr*)&addr, sizeof(addr));
    rfilter[0].can_id = 0x11;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(stSocket_LO, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    while(1)
    {
        stRecv_LO = read(stSocket_LO,&frame, sizeof(frame));
        if(stRecv_LO > 0)
        {
            qDebug()<<"Idata[0]=0x"<<frame.data[0];
        }
    }
    close(stSocket_LO);
    return 0;
}

int Can_Control::socket_connect(const char *m_port)
{
    int stSocket_LO = -1;
    int ret = 0;
    struct sockaddr_can canbud_addr_LO;
    struct ifreq ifreq_LO;
    stSocket_LO = socket(PF_CAN,SOCK_RAW,CAN_RAW);;
    if(stSocket_LO < 0)
    {
        QMessageBox::information(NULL, QString::fromLocal8Bit("ERROR"), QString::fromLocal8Bit("open error"));
        return -1;
    }
    strcpy(ifreq_LO.ifr_name, m_port);
    ret = ioctl(stSocket_LO,SIOCGIFINDEX,&ifreq_LO);
    if(stSocket_LO < 0)
    {
        QMessageBox::information(NULL, QString::fromUtf8("ERROR"), QString::fromUtf8("pipei error"));
        return -1;
    }
    canbud_addr_LO.can_family = AF_CAN;
    canbud_addr_LO.can_ifindex = ifreq_LO.ifr_ifindex;
    ret = bind(stSocket_LO,(struct sockaddr*)&canbud_addr_LO, sizeof(canbud_addr_LO));
    if(ret < 0)
    {
        socket_close(stSocket_LO);
        QMessageBox::information(NULL, QString::fromUtf8("ERROR"), QString::fromUtf8("bangding error"));
        return -1;
    }
    return stSocket_LO;
}

void Can_Control::socket_close(const int sockfd)
{
    if(sockfd != -1)
    {
        close(sockfd);
    }
}

void Can_Control::set_can_filter(const int sockfd)
{
    const int n = 1;
    struct can_filter rfilter[n];
    rfilter[0].can_id = 0x00000000;
    rfilter[0].can_mask = CAN_EFF_MASK;
    setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, rfilter, n*sizeof(rfilter));
    //setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
}

void Can_Control::set_can_loopback(const int sockfd, bool sta)
{
    int loopback = 0;
    if(sta)
        loopback = 1;
    else
        loopback = 0;
    setsockopt(sockfd, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &loopback, sizeof(loopback));
}

int Can_Control::send_frame(int sockfd, const void *data, const int count)
{
    int ret = write(sockfd, (const char *)data, count);
    if(ret != count)
    {
         QMessageBox::information(NULL, QString::fromUtf8("ERROR"), QString::fromUtf8("send error"));
         return 1;
    }
    return 0;
}

int Can_Control::recv_frame(int sockfd, struct can_frame *buf, const int count, const int timeout_ms)
{
    struct timeval tv_timeout;
    tv_timeout.tv_sec = timeout_ms / 1000;
    tv_timeout.tv_usec = (timeout_ms % 1000)*1000;
    fd_set fs_read;
    FD_ZERO(&fs_read);
    FD_SET(sockfd, &fs_read);

    int ret = select((int)sockfd + 1,&fs_read, NULL, NULL, &tv_timeout);
    if(ret == 0)
        return 0;
    if(ret < 0)
        return ret;
    ret = read(sockfd, buf, count);

    if(ret <= 0)
        return -1;
    return ret;
}
