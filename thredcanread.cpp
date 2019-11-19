#include "thredcanread.h"
#include "can_control.h"
#include <QDebug>
thredCanread::thredCanread(QObject *parent):QThread(parent),
    m_bRun(false)
{

}
thredCanread::~thredCanread()
{
    stop();
}

void thredCanread::stop()
{
    m_bRun = false;
    this->wait(3000);
}

void thredCanread::run()
{
    Can_Control can1;
    struct can_frame frame[2];
    frame[0].can_id = 0x11;
    frame[0].can_dlc = 1;
    frame[0].data[0] = '1';
    frame[1].can_id = 0x22;
    frame[1].can_dlc = 1;
    frame[1].data[0] = '2';
    int test;
    byte buf[8];
    test = can1.socket_connect("can1");
    can1.set_can_filter(test);
   // can1.set_can_loopback(test, false);
    m_bRun = true;
    int ret;
    struct can_frame buf1;
    QString data_str;;
    while(m_bRun)
    {
        //can1.can_recv();
        ret = can1.recv_frame(test, &buf1, sizeof(buf1), NULL);
        if(ret > 0)
        {
            qDebug()<<"ID:"<<buf1.can_id;
            qDebug()<<"data_len:"<<buf1.can_dlc;
            data_str = QString(("data: %1 %2 %3 %4 %5 %6 %7 %8")).arg(buf1.data[0]).arg(buf1.data[1]).arg(buf1.data[2])
                        .arg(buf1.data[3]).arg(buf1.data[4]).arg(buf1.data[5]).arg(buf1.data[6]).arg(buf1.data[7]);
            qDebug()<<data_str;
        }
        //can1.send_frame(test, &frame[0], sizeof(frame[0]));
       // sleep(1);
    }
   // can1.send_frame(test, &frame[0], sizeof(frame[0]));
   // can1.socket_close(test);
}
