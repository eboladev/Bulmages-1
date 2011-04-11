#ifndef MYPLUGININFORMES_H
#define MYPLUGININFORMES_H

#include <QObject>
#include <QMenu>
#include <QAction>


class MyPluginBloqConf : public QObject
{
    Q_OBJECT

public:
    MyPluginBloqConf();
    virtual ~MyPluginBloqConf();

public slots:
    virtual void elslot();
    virtual void elslot1();
    virtual void elslot2();
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};


#endif

