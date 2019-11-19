#ifndef THREDCANREAD_H
#define THREDCANREAD_H
#include <QThread>

class thredCanread : public QThread
{
    Q_OBJECT
public:
    explicit thredCanread(QObject *parent = 0);
    ~thredCanread();
    void stop();
protected:
    void run();
    bool m_bRun;
};

#endif // THREDCANREAD_H
