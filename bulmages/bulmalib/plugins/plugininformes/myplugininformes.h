#ifndef MYPLUGININFORMES_H
#define MYPLUGININFORMES_H

#include <QObject>



class MyPluginInformes : public QObject
{
    Q_OBJECT

public:
    MyPluginInformes();
    virtual ~MyPluginInformes();

public slots:
    virtual void elslot1();
};


#endif

