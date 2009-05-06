#ifndef MYPLUGININFORMES_H
#define MYPLUGININFORMES_H

#include <QObject>



class MyPluginBloqConf : public QObject
{
    Q_OBJECT

public:
    MyPluginBloqConf();
    virtual ~MyPluginBloqConf();

public slots:
    virtual void elslot();
    virtual void elslot1();
};


#endif

