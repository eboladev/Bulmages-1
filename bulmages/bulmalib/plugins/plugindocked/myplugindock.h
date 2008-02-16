#ifndef MYPLUGINDOCK_H
#define MYPLUGINDOCK_H

#include <QObject>



class myplugincont : public QObject
{
    Q_OBJECT

public:
    myplugincont();
    virtual ~myplugincont();

public slots:
    virtual void elslot();
    virtual void elslot1();
};


#endif

